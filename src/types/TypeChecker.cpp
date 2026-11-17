//
// Created by SharkBlocks00 on 11/11/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "TypeChecker.h"

#include <ranges>

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

        const auto* mod = dynamic_cast<const ModuleType*>(module.value().get());
        if (!mod) {
            return std::nullopt;
        }

        return mod->getTypes().at(tail);
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
            const auto& _struct = StructType(StructInfo(declaration->getName(), this->iOrigin()));
            this->m_environment->setStruct(declaration->getName(), _struct);
            this->m_preparedStructs.at(declaration) = _struct;
        }
    }

    for (const auto& declaration : declarations) {
        if (this->m_preparedStructs.contains(declaration)) {
            this->iDeclareStructMembers(*declaration, this->m_preparedStructs.at(declaration).getInfo());
        }
    }
}

std::optional<std::string> TypeChecker::iOrigin() const {
    return this->m_currentFile
        ? std::optional{this->m_currentFile->string()}
        : std::nullopt;
}

// Statements

void TypeChecker::statement_ExpressionStatement(const ExpressionStatement &node) const {
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

    if (narrowed != variableType) {
        this->m_environment->narrow(node.getName(), narrowed);
    }

    this->iExport(node.isExported(), node.getName(), variableType);
}

void TypeChecker::statement_DestructureDeclaration(const DestructureDeclaration &node) {
    const auto& value = this->inferValue(node.getInitializer());
    const auto elements = this->unpackTuple(value, node.getTargets().size(), node.getLocation());

    std::set<std::string> seen{};

    for (const auto& target : node.getTargets()) {
        if (seen.contains(target.getName())) {
            throw std::runtime_error{"Variable already exists '" + target.getName() + "'"};
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
    static_cast<void>(this->m_environment->define(name, type, _mutable, kind));
}

void TypeChecker::iExport(const bool &exported, const std::string &name, std::shared_ptr<const FalxType> type) {
    if (exported && this->m_environment == this->m_topScope) {
        this->m_exports.at(name) = type;
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
            if (const auto it = thenState.find(name); it != thenState.end()) {
                parts.push_back(it->second.value());
            }
        }

        if (!elseExits) {
            if (const auto it = elseState.find(name); it != elseState.end()) {
                parts.push_back(it->second.value());
            }
        }

        if (!parts.empty()) {
            this->m_environment->narrow(name, makeUnion(parts));
        }
    }
}

std::map<std::string, std::optional<std::shared_ptr<const FalxType> > > TypeChecker::iState(const std::set<std::string> &names, TypeEnvironment scope) {
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
        if (std::ranges::find(un->getMembers().begin(), un->getMembers().end(), NULL_LITERAL) != un->getMembers().end()) {
            throw std::runtime_error{"Potentially null value!"};
        }
    }

    const auto tmp = {ANY, BOOLEAN_LITERAL, STRING_LITERAL};
    for (const auto& member : membersOf(type)) {
        if (std::ranges::find(tmp.begin(), tmp.end(), member) != tmp.end()) {
            throw std::runtime_error{"Cannot convert to required type!"};
        }
    }
}

void TypeChecker::statement_ForeachStatement(const ForeachStatement &node) {
    const auto& iterable = this->inferValue(node.getIterable());

    if (*iterable != *NULL_LITERAL && isNullable(iterable)) {
        throw std::runtime_error{"Potentially null value!!"};
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

    throw std::runtime_error{"Object not iterable!"};
}

void TypeChecker::statement_FunctionDeclaration(const FunctionDeclaration &node) {
    const std::shared_ptr<const FalxType> function = std::make_shared<FunctionType>(this->iCheckFunction(node, true));
    this->iExport(node.isExported(), node.getName(), function);
}

void TypeChecker::statement_ReturnStatement(const ReturnStatement &node) {
    const auto context = this->m_functions.end();
    auto declared = context->declaredReturn;

    if (!node.getValue()) {
        context->hasBareReturn = true;
        if (declared && *declared.value() != *VOID && !acceptsNull(*declared)) {
            throw std::runtime_error{"Invalid return type!"};
        }
        return;
    }

    auto value = this->inferValue(node.getValue(), declared);

    if (*declared.value() == *VOID) {
        throw std::runtime_error{"Function declared void but returns a value!"};
    }

    if (declared) {
        requireAssignable(*declared, value, "the return value of '" + context->name + "'", node.getValue()->getLocation());
    }

    context->returnTypes.push_back(value);
}


FunctionType TypeChecker::iCheckFunction(const FunctionDeclaration& node, const bool define) {
    std::vector<std::string> names;

    for (const auto& parameter : node.getParameters()) {
        const auto& name = parameter.getName();

        if (std::ranges::find(names, name) != names.end()) {
            throw std::runtime_error{"Duplicate parameter: " + name};
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
                    true
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
            this->m_functions.pop_back();
        } catch (...) {
            this->m_functions.pop_back();
            throw;
        }

        fallsThrough = !terminates(node.getBody());
    });

    auto returnType = NULL_LITERAL;

    if (declaredReturn) {
        if (
            !declaredReturn.value()->equals(*VOID)
            && !acceptsNull(*declaredReturn)
            && fallsThrough
        ) {
            throw std::runtime_error{"Missing return!"};
        }

        returnType = declaredReturn.value();
    }
    else if (context.returnTypes.empty()) {
        returnType = NULL_LITERAL;
    }
    else {
        auto parts = context.returnTypes;

        if (context.hasBareReturn) {
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
    auto declared = this->resolveType(parameter.getTypeAnnotation()? nullptr : parameter.getTypeAnnotation());

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

void TypeChecker::iDefineIn(const std::shared_ptr<TypeEnvironment> &scope, const std::string &name, std::shared_ptr<const FalxType> type, const std::string &kind, const SourceLocation &location) {
    static_cast<void>(scope->define(name, type, false, kind));
}

void TypeChecker::iDeclareStructMembers(const StructDeclaration &node, const StructInfo &info) {
    for (const auto& definition : node.getFields()) {
        auto declared = this->resolveType(definition.getDefaultValue()? definition.getDefaultValue() : ANY);

    }
}
