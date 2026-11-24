//
// Created by SharkBlocks00 on 11/11/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "TypeChecker.h"

#include <algorithm>
#include <fstream>
#include <ranges>
#include <sstream>

#include "../diagnostics/exceptions/TypeExceptions.h"
#include "../diagnostics/exceptions/runtime/FalxRuntimeException.h"
#include "../diagnostics/exceptions/runtime/MethodExceptions.h"
#include "../diagnostics/exceptions/runtime/ModuleExceptions.h"
#include "../diagnostics/exceptions/runtime/OperationExceptions.h"
#include "../diagnostics/exceptions/runtime/VariableExceptions.h"
#include "../lexer/Lexer.h"
#include "../parser/Parser.h"
#include "analysis/StatementAnalysis.h"
#include "builtins/Builtins.h"
#include "relations/TypeRelations.hpp"

std::shared_ptr<TypeEnvironment> makeBuiltinScope() {
    auto scope = TypeEnvironment{};

    for (const auto& name : GLOBAL_FUNCTIONS.names()) {
        static_cast<void>(scope.define(name, *GLOBAL_FUNCTIONS.get(name), false, "builtin"));
    }

    static_cast<void>(scope.define("require", REQUIRE_FUNCTION, false, "builtin"));
    return std::make_shared<TypeEnvironment>(scope);
}

// TypeChecker
void TypeChecker::check(const std::vector<std::shared_ptr<Statement> > &statements) {
    this->checkBlock(statements);
}

void TypeChecker::defineBuiltin(const std::string &name, const std::shared_ptr<const FalxType> &type) const {
    static_cast<void>(this->m_topScope->define(name, type, false, "builtin"));
}

void TypeChecker::checkBlock(const std::vector<std::shared_ptr<Statement> > &statements) {
    this->prepareBlock(statements);

    for (const auto& statement : statements) {
        this->checkStatement(statement);
    }
}

void TypeChecker::checkStatement(const std::shared_ptr<Statement> &statement) {
    if (!statement) {
        return;
    }

    if (const auto* node = dynamic_cast<const ExpressionStatement*>(statement.get())) {
        this->statement_ExpressionStatement(*node);
    } else if (const auto* node = dynamic_cast<const BreakStatement*>(statement.get())) {
        this->statement_BreakStatement(*node);
    } else if (const auto* node = dynamic_cast<const ContinueStatement*>(statement.get())) {
        this->statement_ContinueStatement(*node);
    } else if (const auto* node = dynamic_cast<const BlockStatement*>(statement.get())) {
        this->statement_BlockStatement(*node);
    } else if (const auto* node = dynamic_cast<const VariableDeclaration*>(statement.get())) {
        this->statement_VariableDeclaration(*node);
    } else if (const auto* node = dynamic_cast<const DestructureDeclaration*>(statement.get())) {
        this->statement_DestructureDeclaration(*node);
    } else if (const auto* node = dynamic_cast<const IfStatement*>(statement.get())) {
        this->statement_IfStatement(*node);
    } else if (const auto* node = dynamic_cast<const WhileStatement*>(statement.get())) {
        this->statement_WhileStatement(*node);
    } else if (const auto* node = dynamic_cast<const ForeachStatement*>(statement.get())) {
        this->statement_ForeachStatement(*node);
    } else if (const auto* node = dynamic_cast<const FunctionDeclaration*>(statement.get())) {
        this->statement_FunctionDeclaration(*node);
    } else if (const auto* node = dynamic_cast<const ReturnStatement*>(statement.get())) {
        this->statement_ReturnStatement(*node);
    } else if (const auto* node = dynamic_cast<const StructDeclaration*>(statement.get())) {
        this->statement_StructDeclaration(*node);
    } else {
        throw TypeCheckError("Type checking is not implemented for statement.", statement->getSourceLocation());
    }
}


TypeResolver TypeChecker::resolver() {
    return TypeResolver{
        [this](const std::string& name) {
            return this->iFindStruct(name);
        },
        [this]() {
            return this->m_environment->structNames();
        }
    };
}

std::shared_ptr<const FalxType> TypeChecker::resolveType(const TypeExpressionPtr &annotation, bool allowVoid) {
    return this->resolver().resolve(annotation, allowVoid);
}

std::optional<StructType> TypeChecker::iFindStruct(const std::string &name) const {
    if (name.contains('.')) {
        const auto pos = name.find('.');
        const auto head = name.substr(0, pos);
        const auto tail = name.substr(pos + 1);

        const auto module = this->m_environment->typeOf(head);
        if (!module.has_value()) {
            return std::nullopt;
        }

        const auto* mod = dynamic_cast<const ModuleType*>(module.value().get());
        if (!mod) {
            return std::nullopt;
        }

        const auto& types = mod->getTypes();
        if (const auto it = types.find(tail); it != types.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    return this->m_environment->findStruct(name);
}

void TypeChecker::prepareBlock(const std::vector<std::shared_ptr<Statement> > &statements) {
    const auto names = declaredNames(statements);

    for (const auto& name : names) {
        if (!this->m_environment->getSymbols().contains(name)) {
            this->m_environment->setPending(name);
        }
    }

    std::vector<const StructDeclaration*> declarations;

    for (const auto& statement : statements) {
        if (const auto* expr = dynamic_cast<const StructDeclaration*>(statement.get())) {
            declarations.push_back(expr);
        }
    }

    for (const auto& declaration : declarations) {
        if (!this->m_environment->getStructs().contains(declaration->getName())) {
            const auto _struct = StructType(StructInfo(declaration->getName(), this->iOrigin()));
            this->m_environment->setStruct(declaration->getName(), _struct);
            this->m_preparedStructs.insert_or_assign(declaration, _struct);
        }
    }

    for (const auto& declaration : declarations) {
        if (this->m_preparedStructs.contains(declaration)) {
            this->iDeclareStructMembers(*declaration, this->m_preparedStructs.at(declaration).getInfo());
            this->m_environment->setStruct(declaration->getName(), this->m_preparedStructs.at(declaration));
        }
    }
}

std::optional<std::string> TypeChecker::iOrigin() const {
    return this->m_currentFile
        ? std::optional{this->m_currentFile->filename().string()}
        : std::nullopt;
}

// Statements

void TypeChecker::statement_ExpressionStatement(const ExpressionStatement &node) {
    static_cast<void>(this->infer(node.getExpression()));
}

void TypeChecker::statement_BreakStatement(const BreakStatement &node) {}
void TypeChecker::statement_ContinueStatement(const ContinueStatement &node) {}

void TypeChecker::statement_BlockStatement(const BlockStatement &node) {
    std::map<std::string, std::shared_ptr<const FalxType>> carried;

    this->scope([&](const TypeEnvironment& block) {
        this->checkBlock(node.getStatements());

        for (const auto& [name, type] : block.getNarrowed()) {
            if (!block.getSymbols().contains(name)) {
                carried.emplace(name, type);
            }
        }
    });

    this->m_environment->narrowAll(carried);
}

void TypeChecker::statement_VariableDeclaration(const VariableDeclaration &node) {
    const auto& declared = node.getTypeAnnotation()? this->resolveType(node.getTypeAnnotation()) : nullptr;
    const auto value = this->inferValue(node.getInitializer(), declared);

    std::shared_ptr<const FalxType> variableType;
    std::shared_ptr<const FalxType> narrowed;

    if (declared) {
        requireAssignable(declared, value, "variable '" + node.getName() + "'", node.getInitializer()->getLocation());
        variableType = declared;
        narrowed = flowType(declared, value);
    } else {
        variableType = widen(value, node.isMutable());
        narrowed = variableType;
    }

    this->iDefine(node.getName(), variableType, node.isMutable(), "variable", node.getSourceLocation());

    if (*narrowed != *variableType) {
        this->m_environment->narrow(node.getName(), narrowed);
    }

    this->iExport(node.isExported(), node.getName(), variableType);
}

void TypeChecker::statement_DestructureDeclaration(const DestructureDeclaration &node) {
    const auto& value = this->inferValue(node.getInitializer());
    const auto elements = this->unpackTuple(value, static_cast<int>(node.getTargets().size()), node.getLocation());

    std::set<std::string> seen{};

    for (const auto& target : node.getTargets()) {
        if (seen.contains(target.getName())) {
            throw VariableAlreadyExistsException(target.getName(), target.getLocation());
        }
        seen.insert(target.getName());
    }

    for (std::size_t i = 0; i < node.getTargets().size(); ++i) {
        const auto& target = node.getTargets()[i];
        const auto& element = elements.at(i);
        const auto& variableType = widen(element, node.isMutable());

        this->iDefine(target.getName(), variableType, node.isMutable(), "variable", target.getLocation());
        this->iExport(node.exported(), target.getName(), variableType);
    }
}

void TypeChecker::iDefine(const std::string &name, const std::shared_ptr<const FalxType> &type, const bool _mutable, const std::string &kind, const SourceLocation &location) const {
    try {
        static_cast<void>(this->m_environment->define(name, type, _mutable, kind));
    } catch (FalxException& error) {
        throw error.withLocation(location);
    }
}

void TypeChecker::iExport(const bool exported, const std::string &name, const std::shared_ptr<const FalxType>& type) {
    if (exported && this->m_environment == this->m_topScope) {
        this->m_exports[name] = type;
    }
}

void TypeChecker::statement_IfStatement(const IfStatement &node) {
    static_cast<void>(this->inferValue(node.getCondition()));
    const auto& [whenTrue, whenFalse] = this->factsFor(node.getCondition());

    const auto& branches = node.getElseBody()? std::vector{node.getElseBody()} : std::vector<std::shared_ptr<Statement>>{};
    std::set<std::string> interesting;

    for (const auto &name: whenTrue | std::views::keys) {
        interesting.insert(name);
    }

    for (const auto& name : whenFalse | std::views::keys) {
        interesting.insert(name);
    }

    const auto bodyNames = assignedNames(node.getStatements());
    interesting.insert(bodyNames.begin(), bodyNames.end());

    const auto branchNames = assignedNames(branches);
    interesting.insert(branchNames.begin(), branchNames.end());

    bool thenExits{false};
    bool elseExits{false};

    decltype(this->iState(interesting, *this->m_environment)) thenState;
    decltype(this->iState(interesting, *this->m_environment)) elseState;

    this->scope([&](const TypeEnvironment& thenScope) {
        this->checkBlock(node.getStatements());

        thenExits = terminates(node.getStatements(), true);
        thenState = this->iState(interesting, thenScope);
    }, whenTrue);

    this->scope([&](const TypeEnvironment& elseScope) {
        if (node.getElseBody()) {
            this->checkStatement(node.getElseBody());
        }

        elseExits = node.getElseBody() && terminates({node.getElseBody()}, true);
        elseState = this->iState(interesting, elseScope);
    }, whenFalse);

    for (const auto& name : interesting) {
        std::vector<std::shared_ptr<const FalxType>> parts;

        if (!thenExits) {
            if (const auto it = thenState.find(name); it != thenState.end() && it->second.has_value()) {
                parts.push_back(it->second.value());
            }
        }

        if (!elseExits) {
            if (const auto it = elseState.find(name); it != elseState.end() && it->second.has_value()) {
                parts.push_back(it->second.value());
            }
        }

        if (!parts.empty()) {
            this->m_environment->narrow(name, makeUnion(parts));
        }
    }
}

std::map<std::string, std::optional<std::shared_ptr<const FalxType> > > TypeChecker::iState(const std::set<std::string> &names, const TypeEnvironment& scope) {
    std::map<std::string, std::optional<std::shared_ptr<const FalxType> > > result;

    for (const auto& name : names) {
        if (!scope.getSymbols().contains(name)) {
            result.insert({name, scope.typeOf(name)});
        }
    }

    return result;
}

void TypeChecker::iForgetAssigned(TypeEnvironment &scope, const std::set<std::string> &names) {
    for (const auto& name : names) {
        if (auto symbol = scope.lookup(name)) {
            scope.narrow(name, symbol.value()->getType());
        }
    }
}

void TypeChecker::statement_WhileStatement(const WhileStatement &node) {
    const auto assigned = assignedNames(node.getBody());

    Facts whenFalse;

    this->scope([&](TypeEnvironment& loop) {
        this->iForgetAssigned(loop, assigned);

        const auto condition = this->inferValue(node.getCondition());

        iRequireCondition(condition, node.getCondition(), node.getSourceLocation());

        auto [whenTrue, falseFacts] = this->factsFor(node.getCondition());
        whenFalse = std::move(falseFacts);

        this->scope([&](const TypeEnvironment&) {
            this->checkBlock(node.getBody());
        }, whenTrue);
    });

    this->iForgetAssigned(*this->m_environment, assigned);

    if (!containsBreak(node.getBody())) {
        this->m_environment->narrowAll(whenFalse);
    }
}

void TypeChecker::iRequireCondition(const std::shared_ptr<const FalxType> &type, const std::shared_ptr<Expression> &condition, const SourceLocation &location) {
    if (const auto* un = dynamic_cast<const UnionType*>(type.get())) {
        if (std::ranges::any_of(un->getMembers(), [](const auto& m) { return *m == *NULL_LITERAL; })) {
            throw NullableValueException(
                describe(condition) + " may be null ('" + type->toString() + "'), so it cannot be used as a condition",
                location
            );
        }
    }

    for (const auto& member : membersOf(type)) {
        if (*member != *ANY && *member != *BOOLEAN_LITERAL && *member != *STRING_LITERAL) {
            throw CannotConvertToTypeException(member->toString(), "bool", location);
        }
    }
}

void TypeChecker::statement_ForeachStatement(const ForeachStatement &node) {
    const auto& iterable = this->inferValue(node.getIterable());

    if (*iterable != *NULL_LITERAL && isNullable(iterable)) {
        throw NullableValueException(
            describe(node.getIterable()) + " may be null ('" + iterable->toString() + "'), so it cannot be iterated",
            node.getSourceLocation()
        );
    }

    std::vector<std::shared_ptr<const FalxType>> members;
    for (const auto& member : membersOf(iterable)) {
        members.push_back(iElementType(member, node));
    }

    auto element = makeUnion(members);
    auto assigned = assignedNames(node.getBody());

    this->scope([&](TypeEnvironment& loop) {
        this->iForgetAssigned(loop, assigned);
        static_cast<void>(loop.define(node.getVariable(), element, false));
        this->checkBlock(node.getBody());
    }, std::nullopt);

    this->iForgetAssigned(*this->m_environment, assigned);
}

std::shared_ptr<const FalxType> TypeChecker::iElementType(const std::shared_ptr<const FalxType> &iterable, const ForeachStatement &node) {
    if (*iterable == *ANY) {
        return ANY;
    }

    if (*iterable == *STRING_LITERAL) {
        return STRING_LITERAL;
    }

    if (const auto* array = dynamic_cast<const ArrayType*>(iterable.get())) {
        return array->getElementType();
    }

    if (const auto* map = dynamic_cast<const MapType*>(iterable.get())) {
        return map->getValueType();
    }

    if (const auto* tuple = dynamic_cast<const TupleType*>(iterable.get())) {
        return makeUnion(tuple->getElementTypes());
    }

    throw ObjectNotIterableException(iterable->toString(), node.getSourceLocation());
}

void TypeChecker::statement_FunctionDeclaration(const FunctionDeclaration &node) {
    const std::shared_ptr<const FalxType> function = std::make_shared<FunctionType>(this->iCheckFunction(node, true));
    this->iExport(node.isExported(), node.getName(), function);
}

void TypeChecker::statement_ReturnStatement(const ReturnStatement &node) {
    auto& context = this->m_functions.back();
    auto declared = context.declaredReturn;

    if (!node.getValue()) {
        context.hasBareReturn = true;
        if (declared && *declared.value() != *VOID && !acceptsNull(*declared)) {
            throw TypeMismatchException(
                "Function '" + context.name + "' must return a value of type '" + declared.value()->toString() + "'",
                node.getSourceLocation()
            );
        }
        return;
    }

    auto value = this->inferValue(node.getValue(), declared);

    if (declared && *declared.value() == *VOID) {
        throw TypeMismatchException(
            "Function '" + context.name + "' is declared 'void' but returns a value",
            node.getSourceLocation()
        );
    }

    if (declared) {
        requireAssignable(*declared, value, "the return value of '" + context.name + "'", node.getValue()->getLocation());
    }

    context.returnTypes.push_back(value);
}


FunctionType TypeChecker::iCheckFunction(const FunctionDeclaration& node, const bool define) {
    std::vector<std::string> names;

    for (const auto& parameter : node.getParameters()) {
        const auto& name = parameter.getName();

        if (std::ranges::find(names, name) != names.end()) {
            throw DuplicateParameterException(name, node.getSourceLocation());
        }

        names.push_back(name);
    }

    std::optional<std::shared_ptr<const FalxType>> declaredReturn;

    if (node.getReturnType()) {
        declaredReturn = this->resolveType(node.getReturnType(), true);
    }

    auto outer = this->m_environment;

    std::vector<std::shared_ptr<const FalxType>> parameterTypes;
    std::optional<std::size_t> required = std::nullopt;

    FunctionContext context{node.getName(), declaredReturn};

    bool fallsThrough{false};

    this->scope([&](TypeEnvironment& bodyScope) {
        this->prepareBlock(node.getBody());

        for (std::size_t position = 0;
             position < node.getParameters().size();
             ++position) {

            const auto& parameter = node.getParameters()[position];

            parameterTypes.push_back(this->iParameterType(parameter));

            if (parameter.getDefaultValue() && !required.has_value()) {
                required = position;
            }

            static_cast<void>(
                bodyScope.define(
                    parameter.getName(),
                    parameterTypes.back(),
                    true,
                    "parameter"
                )
            );
        }

        const std::shared_ptr<const FalxType> provisional =
            std::make_shared<FunctionType>(
                parameterTypes,
                declaredReturn.value_or(ANY),
                required,
                names
            );

        if (define) {
            this->iDefineIn(
                outer,
                node.getName(),
                provisional,
                "function",
                node.getSourceLocation()
            );
        }

        this->m_functions.push_back(context);

        try {
            this->checkBlock(node.getBody());
            context = this->m_functions.back();
            this->m_functions.pop_back();
        } catch (...) {
            this->m_functions.pop_back();
            throw;
        }

        fallsThrough = !terminates(node.getBody());
    }, std::nullopt, true);

    auto returnType = NULL_LITERAL;

    if (declaredReturn) {
        if (
            !declaredReturn.value()->equals(*VOID)
            && !acceptsNull(*declaredReturn)
            && fallsThrough
        ) {
            throw MissingReturnException(node.getName(), declaredReturn.value()->toString(), node.getSourceLocation());
        }

        returnType = declaredReturn.value();
    }
    else if (context.returnTypes.empty()) {
        returnType = NULL_LITERAL;
    }
    else {
        auto parts = context.returnTypes;

        if (context.hasBareReturn || fallsThrough) {
            parts.push_back(NULL_LITERAL);
        }

        returnType = makeUnion(parts);
    }

    const std::shared_ptr<const FunctionType> function =
        std::make_shared<FunctionType>(
            parameterTypes,
            returnType,
            required,
            names
        );

    if (define) {
        outer->redefine(node.getName(), function);
    }

    return *function;
}

std::shared_ptr<const FalxType> TypeChecker::iParameterType(const ParameterDefinition &parameter) {
    auto declared = parameter.getTypeAnnotation() ? this->resolveType(parameter.getTypeAnnotation()) : nullptr;

    if (!parameter.getDefaultValue()) {
        return declared? declared : ANY;
    }

    auto def = this->inferValue(parameter.getDefaultValue(), declared);

    if (!declared) {
        return widen(def, true);
    }

    requireAssignable(declared, def, "parameter '" + parameter.getName() + "'", parameter.getDefaultValue()->getLocation());
    return declared;
}

void TypeChecker::iDefineIn(const std::shared_ptr<TypeEnvironment> &scope, const std::string &name, const std::shared_ptr<const FalxType>& type, const std::string &kind, const SourceLocation &location) {
    try {
        static_cast<void>(scope->define(name, type, false, kind));
    } catch (FalxException& error) {
        throw error.withLocation(location);
    }
}

void TypeChecker::iDeclareStructMembers(const StructDeclaration &node, StructInfo &info) {
    for (const auto& definition : node.getFields()) {
        auto declared = definition.getTypeAnnotation() ? this ->resolveType(definition.getTypeAnnotation()) : ANY;
        info.setField(definition.getName(), FieldInfo{declared, definition.getDefaultValue() != nullptr});
    }

    for (const auto& method : node.getMethods()) {
        std::vector<std::shared_ptr<const FalxType>> parameters;
        std::vector<std::string> parameterNames;
        for (const auto& p : method->getParameters()) {
            parameters.push_back(p.getTypeAnnotation()? this->resolveType(p.getTypeAnnotation()) : ANY);
            parameterNames.push_back(p.getName());
        }
        auto returns = method->getReturnType()? this->resolveType(method->getReturnType(), true) : ANY;
        const auto it = std::ranges::find_if(method->getParameters(), [](const auto& parameter) { return parameter.getDefaultValue() != nullptr; });

        std::optional<std::size_t> required;

        if (it != method->getParameters().end()) {
            required = static_cast<std::size_t>(
                std::distance(method->getParameters().begin(), it)
            );
        }

        info.setMethod(method->getName(), FunctionType(parameters, returns, required, parameterNames));
    }
}

void TypeChecker::statement_StructDeclaration(const StructDeclaration &node) {
    std::shared_ptr<StructType> structType;

    const auto it = this->m_preparedStructs.find(&node);

    if (it != this->m_preparedStructs.end()) {
        structType = std::make_shared<StructType>(it->second);
    }

    if (!structType) {
        structType = std::make_shared<StructType>(StructInfo(node.getName(), this->iOrigin()));

        this->iDeclareStructMembers(node, structType->getInfo());
    }

    auto& info = structType->getInfo();

    for (const auto& definition : node.getFields()) {
        info.setField(definition.getName(), FieldInfo{this->iFieldType(definition), definition.getDefaultValue() != nullptr});
    }

    const std::shared_ptr<const FalxType> constructor = std::make_shared<FunctionType>(this->iConstructorType(node, *structType));

    TypeChecker::iDefineIn(this->m_environment, node.getName(), constructor, "struct", node.getSourceLocation());

    auto members = std::make_shared<TypeEnvironment>(this->m_environment, false);

    for (const auto& [name, fieldInfo] : info.getFields()) {
        static_cast<void>(members->define(name, fieldInfo.getType(), true, "field"));
    }

    for (const auto& [name, method] : info.getMethods()) {
        static_cast<void>(members->define(name, std::make_shared<FunctionType>(method), false, "function"));
    }

    this->scope(std::move(members), [&](TypeEnvironment& bodyScope) {
        for (const auto& method : node.getMethods()) {
            auto function = this->iCheckFunction(*method, false);

            info.setMethod(method->getName(), function);

            bodyScope.redefine(method->getName(), std::make_shared<FunctionType>(function));
        }
    });

    this->m_environment->setStruct(node.getName(), *structType);

    if (node.exported() && this->m_environment == this->m_topScope) {
        this->m_exports[node.getName()] = constructor;
        this->m_exportedStructs.insert_or_assign(node.getName(), *structType);
    }
}

std::shared_ptr<const FalxType> TypeChecker::iFieldType(const FieldDefinition &field) {
    auto declared = field.getTypeAnnotation()? this->resolveType(field.getTypeAnnotation()) : nullptr;

    if (!field.getDefaultValue()) {
        return declared? declared : ANY;
    }

    const auto def = this->inferValue(field.getDefaultValue(), declared);

    if (!declared) {
        return widen(def, true);
    }

    requireAssignable(declared, def, "field '" + field.getName() + "'", field.getDefaultValue()->getLocation());
    return declared;
}

FunctionType TypeChecker::iConstructorType(const StructDeclaration &node, const StructType &_struct) {
    const auto& fields = _struct.getInfo().getFields();

    std::size_t required = 0;
    std::size_t index = 0;
    std::vector<std::shared_ptr<const FalxType>> types;
    std::vector<std::string> names;

    for (const auto& [name, fieldInfo] : fields) {
        if (!fieldInfo.isDefault() && !acceptsNull(fieldInfo.getType())) {
            required = index + 1;
        }
        types.push_back(fieldInfo.getType());
        names.push_back(name);
        index++;
    }

    return FunctionType{
        types,
        std::make_shared<StructType>(_struct),
        required,
        names
    };
}

std::shared_ptr<const FalxType> TypeChecker::loadModule(const std::string &name, const SourceLocation &location) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    while (true) {
        const auto pos = name.find("::", start);
        if (pos == std::string::npos) {
            parts.push_back(name.substr(start));
            break;
        }
        parts.push_back(name.substr(start, pos - start));
        start = pos + 2;
    }

    if (parts.size() == 1) {
        return std::make_shared<ModuleType>(this->iLoadFile(name, this->iResolveUserModule(name), location));
    }

    std::string subPath;
    for (std::size_t i = 1; i < parts.size(); ++i) {
        if (i > 1) {
            subPath += "/";
        }
        subPath += parts[i];
    }

    if (parts[0] == "_internal") {
        const auto module = internalModule(subPath);
        if (!module.has_value()) {
            throw ModuleNotFoundException(name, "internal module directory").withLocation(location);
        }
        return std::make_shared<ModuleType>(*module);
    }

    if (parts[0] == "std") {
        const auto path = std::filesystem::weakly_canonical(this->m_stdlibDir / (subPath + ".flx"));
        const auto rel = path.lexically_relative(this->m_stdlibDir);
        const bool isInside = !rel.empty() && rel.native().rfind(L"..", 0) != 0;
        if (!isInside || !std::filesystem::is_regular_file(path)) {
            throw ModuleNotFoundException(name, path.string()).withLocation(location);
        }
        return std::make_shared<ModuleType>(this->iLoadFile(name, path, location));
    }

    throw ModuleNotFoundException(name, parts[0]).withLocation(location);
}

std::filesystem::path TypeChecker::iResolveUserModule(const std::string &name) const {
    const auto base = this->m_currentFile ? this->m_currentFile->parent_path() : *this->m_projectDir;
    const auto path = std::filesystem::weakly_canonical(base / (name + ".flx"));
    const auto rel = path.lexically_relative(*this->m_projectDir);
    const bool isInside = !rel.empty() && rel.native().rfind(L"..", 0) != 0;

    if (!isInside || !std::filesystem::is_regular_file(path)) {
        throw ModuleNotFoundException(name, path.string());
    }

    return path;
}

ModuleType TypeChecker::iLoadFile(const std::string &name, const std::filesystem::path &path, const SourceLocation &location) {
    if (std::ranges::find(this->m_cache->loading, path) != this->m_cache->loading.end()) {
        throw CircularModuleDependencyException(path, this->m_cache->loading).withLocation(location);
    }

    if (const auto it = this->m_cache->modules.find(path); it != this->m_cache->modules.end()) {
        return it->second;
    }

    this->m_cache->loading.push_back(path);
    TypeChecker checker(this->m_projectDir, path, this->m_cache, true);
    try {
        std::ifstream file(path);
        std::ostringstream buffer;
        buffer << file.rdbuf();

        const auto tokens = Lexer(buffer.str(), path.filename().string()).lex();
        const auto statements = Parser(tokens).parse();
        checker.check(statements);
        this->m_cache->loading.pop_back();
    } catch (...) {
        this->m_cache->loading.pop_back();
        throw;
    }

    std::map<std::string, const StructType> exportedTypes(checker.m_exportedStructs.begin(), checker.m_exportedStructs.end());
    ModuleType module(name, checker.m_exports, exportedTypes);
    this->m_cache->modules.insert_or_assign(path, module);
    return module;
}
