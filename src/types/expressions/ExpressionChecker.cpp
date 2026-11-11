//
// Created by SharkBlocks00 on 11/7/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "ExpressionChecker.hpp"

#include <algorithm>
#include <ranges>

#include "../TypeRelations.hpp"
#include "../../ast/expressions/FunctionCallExpression.h"
#include "../../ast/expressions/GetExpression.h"
#include "../../ast/expressions/IndexExpression.h"
#include "../../ast/expressions/OptionalChainExpression.h"
#include "../../ast/expressions/VariableExpression.h"
#include "../../ast/expressions/literals/BooleanLiteral.h"
#include "../../ast/expressions/literals/NullLiteral.h"
#include "../../ast/expressions/literals/NumberLiteral.h"
#include "../../ast/expressions/literals/StringLiteral.h"
#include "../../helpers/StringUtils.h"
#include "../builtins/Builtins.h"

std::optional<std::shared_ptr<const FalxType>> binaryResult(const TokenKind& op, const std::shared_ptr<const FalxType>& left, const std::shared_ptr<const FalxType>& right) {
    const bool bothNumeric = std::ranges::find(NUMERIC, left) != NUMERIC.end() && std::ranges::find(NUMERIC, right) != NUMERIC.end();
    const auto numericResult = *FLOAT_LITERAL == *left || *FLOAT_LITERAL == *right? FLOAT_LITERAL : INTEGER_LITERAL;

    if (op == TokenKind::PLUS) {
        if (bothNumeric) {
            return numericResult;
        }
        if (std::ranges::find(NUMERIC, left) != NUMERIC.end() && *right == *STRING_LITERAL) {
            return STRING_LITERAL;
        }

        if (const auto tmp = {STRING_LITERAL, INTEGER_LITERAL, FLOAT_LITERAL, BOOLEAN_LITERAL}; *left == *STRING_LITERAL && std::ranges::find(tmp, right) != tmp.end()) {
            return STRING_LITERAL;
        }
    }

    else if (op == TokenKind::MINUS || op == TokenKind::SLASH || op == TokenKind::PERCENT) {
        if (bothNumeric) {
            return numericResult;
        }
    }

    else if (op == TokenKind::STAR) {
        if (bothNumeric) {
            return numericResult;
        }
        if (std::ranges::find(NUMERIC, left) != NUMERIC.end() && *right == *STRING_LITERAL) {
            return STRING_LITERAL;
        }
    } else if (std::ranges::find(COMPARISONS, op) != COMPARISONS.end()) {
        if (bothNumeric) {
            return BOOLEAN_LITERAL;
        }
    }

    return nullptr;
}

std::shared_ptr<const FalxType> widen(std::shared_ptr<const FalxType> type, std::shared_ptr<const FalxType> mut) {
    if (*type == *NEVER || *type == *NULL_LITERAL) {
        return ANY;
    }

    if (const auto* array = dynamic_cast<const ArrayType*>(type.get());
        array && (*array->getElementType() == *NEVER || *array->getElementType() == *NULL_LITERAL)
    ) {
        return std::make_shared<ArrayType>(ANY);
    }

    if (const auto* map = dynamic_cast<const MapType*>(type.get());
        map && (*map->getValueType() == *NEVER || *map->getValueType() == *NULL_LITERAL)
    ) {
        return std::make_shared<MapType>(map->getKeyType(), ANY);
    }

    return type;
}

std::shared_ptr<const FalxType> flowType(std::shared_ptr<const FalxType> declared, std::shared_ptr<const FalxType> value) {
    if (*value == *ANY || !isNullable(declared)) {
        return declared;
    }

    if (isAssignable(declared, value)) {
        return value;
    }

    return declared;
}

// ExpressionChecker

std::shared_ptr<const FalxType> ExpressionChecker::infer(const std::shared_ptr<Expression> &expression, std::optional<std::shared_ptr<const FalxType> > expected) {
    if (!expression) {
        throw std::invalid_argument("Expression cannot be null");
    }

    if (auto node = std::dynamic_pointer_cast<NumberLiteral>(expression)) {
        return expression_NumberLiteral(*node, expected);
    }

    if (auto node = std::dynamic_pointer_cast<StringLiteral>(expression)) {
        return expression_StringLiteral(*node, expected);
    }

    if (auto node = std::dynamic_pointer_cast<BooleanLiteral>(expression)) {
        return expression_BooleanLiteral(*node, expected);
    }

    if (auto node = std::dynamic_pointer_cast<NullLiteral>(expression)) {
        return expression_NullLiteral(*node, expected);
    }

    throw std::runtime_error{"Type checking is not implemented for this expression type."};
}

std::shared_ptr<const FalxType> ExpressionChecker::inferValue(const std::shared_ptr<Expression> &expression, std::optional<std::shared_ptr<const FalxType> > expected) const {
    const auto type = infer(expression, expected);

    if (type == VOID) {
        throw std::runtime_error{this->describe(expression) + " does not return a value."};
    }

    return type;
}

template<typename Callback>
auto ExpressionChecker::scope(Callback &&callback, const std::optional<Facts> facts, const bool functionBoundary) -> std::invoke_result_t<Callback, TypeEnvironment &> {
    using Result = std::invoke_result_t<Callback, TypeEnvironment&>;

    static_assert(!std::is_reference_v<Result>, "Scope callbacks must not return references.");

    TypeEnvironment previous = m_environment;

    try {
        m_environment = *previous.child(functionBoundary);

        if (facts.has_value()) {
            m_environment.narrowAll(*facts);
        }

        if constexpr (std::is_void_v<Result>) {
            std::invoke(
                std::forward<Callback>(callback),
                m_environment);

            m_environment = std::move(previous);
        }
        else {
            Result result = std::invoke(
                std::forward<Callback>(callback),
                m_environment);

            m_environment = std::move(previous);

            return result;
        }
    } catch (...) {
        m_environment = std::move(previous);
        throw;
    }
    return nullptr;
}

std::pair<Facts, Facts> ExpressionChecker::factsFor(const std::shared_ptr<Expression> &condition) {
    return conditionFacts(condition, [this](const auto& name) { return m_environment.typeOf(name);});
}


std::string ExpressionChecker::describe(const std::shared_ptr<Expression> &expression) {
    if (const auto* var = dynamic_cast<const VariableExpression*>(expression.get())) {
        return "'"+ var->getName() + "'";
    }

    if (const auto* get = dynamic_cast<const GetExpression*>(expression.get())) {
        return "'" + source(expression) + "'";
    }

    if (const auto* fun = dynamic_cast<const FunctionCallExpression*>(expression.get())) {
        return "the result of '" + source(expression) + "'";
    }

    return "this expression";
}

std::string ExpressionChecker::source(const std::shared_ptr<Expression> &expression) {
    if (const auto* var = dynamic_cast<const VariableExpression*>(expression.get())) {
        return var->getName();
    }

    if (const auto* get = dynamic_cast<const GetExpression*>(expression.get())) {
        std::ostringstream result;
        result << source(expression);
        if (get->isOptional()) {
            result << "?.";
        } else {
            result << ".";
        }
        result << get->getProperty();
        return result.str();
    }

    if (const auto* fun = dynamic_cast<const FunctionCallExpression*>(expression.get())) {
        return source(fun->getCallee()) + "(...)";
    }

    if (const auto* idx = dynamic_cast<const IndexExpression*>(expression.get())) {
        return source(idx->getObject()) + "[...]";
    }

    if (const auto* opt = dynamic_cast<const OptionalChainExpression*>(expression.get())) {
        return source(opt->getChain());
    }

    return "...";
}

void ExpressionChecker::requireAssignable(const std::shared_ptr<const FalxType> &expected, const std::shared_ptr<const FalxType> &actual, const std::string &what, std::optional<SourceLocation> location) {
    if (!isAssignable(expected, actual)) {
        throw std::runtime_error{"Cannot assign '" + actual->toString() + "' to " + what + " of type '" + expected->toString() + "'"};
    }
}

std::shared_ptr<const FalxType> ExpressionChecker::nonNullReceiver(const std::shared_ptr<const FalxType> &type, const Expression& node, const std::shared_ptr<Expression> &receiver, const std::string &action, const bool &optional) {
    if (*type == *ANY || !isNullable(type)) {
        return type;
    }

    if (optional) {
        if (!this->m_chains.empty()) {
            this->m_chains.back() = true;
        }
        return nonNull(type);
    }

    if (*type == *NULL_LITERAL) {
        throw std::runtime_error{"Cannot " + action + " on null."};
    }

    throw std::runtime_error{describe(receiver) + " may be null, so you cannot " + action + " on it."};
}


// Literals

std::shared_ptr<const FalxType> ExpressionChecker::expression_NumberLiteral(const NumberLiteral &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    if (std::holds_alternative<int>(node.getValue())) {
        return INTEGER_LITERAL;
    }
    return FLOAT_LITERAL;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_StringLiteral(const StringLiteral &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    return STRING_LITERAL;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_BooleanLiteral(const BooleanLiteral &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    return BOOLEAN_LITERAL;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_NullLiteral(const NullLiteral &node, const std::optional<std::shared_ptr<const FalxType>>& expected) {
    return NULL_LITERAL;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_ArrayLiteral(const ArrayLiteral &node, const std::optional<std::shared_ptr<const FalxType> > &expected) const {
    const auto elementHint = this->hint<ArrayType>(expected, [](const ArrayType& t) { return t.getElementType(); });

    if (node.getElements().empty()) {
        return elementHint ? std::make_shared<ArrayType>(*elementHint) : std::make_shared<ArrayType>(ANY);
    }

    std::vector<std::shared_ptr<const FalxType>> elementTypes;

    for (const auto& element : node.getElements()) {
        elementTypes.push_back(inferValue(element, elementHint));
    }

    if (elementHint && std::ranges::all_of(std::as_const(elementTypes), [&elementHint](const auto& t) {
        return isAssignable(*elementHint, t);
    })) {
        return std::make_shared<ArrayType>(*elementHint);
    }

    return std::make_shared<ArrayType>(makeUnion(elementTypes));
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_MapLiteral(const MapLiteral &node, const std::optional<std::shared_ptr<const FalxType> >& expected) const {
    const auto valueHint = this->hint<MapType>(expected, [](const MapType& t) { return t.getValueType(); });
    if (node.getValues().empty()) {
        return valueHint? std::make_shared<MapType>(STRING_LITERAL, valueHint.value()) : std::make_shared<MapType>(STRING_LITERAL, ANY);
    }

    std::vector<std::shared_ptr<const FalxType>> valueTypes;

    for (const auto&[_, snd] : node.getValues()) {
        valueTypes.push_back(this->inferValue(snd, valueHint));
    }

    if (valueHint && std::all_of(valueTypes.cbegin(), valueTypes.cend(),
        [&valueHint](const auto& t) { return isAssignable(*valueHint, t); }
    )) {
        return std::make_shared<MapType>(STRING_LITERAL, valueHint.value());
    }

    const std::set<std::shared_ptr<const FalxType>> distinct{valueTypes.begin(), valueTypes.end()};
    return distinct.size() == 1? std::make_shared<MapType>(STRING_LITERAL, valueTypes[0]) : std::make_shared<MapType>(STRING_LITERAL, ANY);
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_TupleLiteral(const TupleLiteral& node, const std::optional<std::shared_ptr<const FalxType>>& expected) const {
    std::vector<std::shared_ptr<const FalxType>> hints;

    if (expected && *expected) {
        if (const auto tupleType =
                std::dynamic_pointer_cast<const TupleType>(*expected);
            tupleType &&
            tupleType->getElementTypes().size() == node.getElements().size()) {
            hints = tupleType->getElementTypes();
            }
    }

    std::vector<std::shared_ptr<const FalxType>> elementTypes;

    for (std::size_t i = 0; i < node.getElements().size(); ++i) {
        std::optional<std::shared_ptr<const FalxType>> elementHint;

        if (!hints.empty()) {
            elementHint = hints[i];
        }

        elementTypes.push_back(
            inferValue(node.getElements()[i], elementHint)
        );
    }

    return std::make_shared<TupleType>(std::move(elementTypes));
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_VariableExpression(const VariableExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) const {
    if (const auto type = this->m_environment.typeOf(node.getName())) {
        return *type;
    }

    if (this->m_environment.isPending(node.getName())) {
        return ANY;
    }

    throw std::runtime_error{"Undefined variable '" + node.getName() + "'"};
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_AssignmentExpression(const AssignmentExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    const auto symbol = this->m_environment.lookup(node.getName());

    if (!symbol) {
        if (this->m_environment.isPending(node.getName())) {
            return this->inferValue(node.getValue());
        }
        throw std::runtime_error{"Undefined variable '" + node.getName() + "'"};
    }

    if (!symbol.value()->isMutable()) {
        throw std::runtime_error{"Cannot assign to constant '" + node.getName() + "'"};
    }

    const auto value = this->inferValue(node.getValue(), symbol.value()->getType());
    this->requireAssignable(symbol.value()->getType(), value, "variable '" + node.getName() + "'", node.getValue()->getLocation());
    this->m_environment.narrow(node.getName(), flowType(symbol.value()->getType(), value));
    return value;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_DestructureAssignmentExpression(const DestructureAssignment &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    const auto value = this->inferValue(node.getValue());
    const auto elementTypes = ExpressionChecker::unpackTuple(value, node.getTargets().size(), node.getLocation());

    for (std::size_t i = 0; i < node.getTargets().size() && i < elementTypes.size(); ++i) {
        auto& target = node.getTargets()[i];
        auto& element = elementTypes[i];

        auto symbol = this->m_environment.lookup(target.getName());

        if (symbol == nullptr) {
            if (this->m_environment.isPending(target.getName())) {
                continue;
            }

            throw std::runtime_error{"Undefined variable '" + target.getName() + "'"};
        }

        if (!symbol.value()->isMutable()) {
            throw std::runtime_error{"Cannot assign to constant '" + target.getName() + "'"};
        }

        this->requireAssignable(symbol.value()->getType(), element, "variable '" + target.getName() + "'", target.getLocation());
        this->m_environment.narrow(target.getName(), flowType(symbol.value()->getType(), value));
    }
    return value;
}

std::vector<std::shared_ptr<const FalxType> > ExpressionChecker::unpackTuple(const std::shared_ptr<const FalxType> &value, const int &count, const SourceLocation &location) {
    if (*value == *ANY) {
        std::vector<std::shared_ptr<const FalxType>> ret;
        for (std::size_t i = 0; i < count; ++i) {
            ret.push_back(ANY);
        }
        return ret;
    }

    if (const auto* tuple = dynamic_cast<const TupleType*>(value.get())) {
        if (tuple->getElementTypes().size() != count) {
            throw std::runtime_error{"Invalid count for destructiring!"};
        }
        return tuple->getElementTypes();
    }

    if (isNullable(value) && *nonNull(value) != *NEVER && dynamic_cast<const TupleType*>(nonNull(value).get())) {
        throw std::runtime_error{"Cannot destructure '" + value->toString() + "': it may be null"};
    }

    throw std::runtime_error{"Unexpectedly found a tuple!"};
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_BinaryExpression(const BinaryExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) const {
    const auto op = node.getOperator().getTokenKind();
    const auto left = this->inferValue(node.getLeft());
    const auto right = this->inferValue(node.getRight());

    if (std::ranges::find(EQUALITIES, op) != EQUALITIES.end()) {
        return BOOLEAN_LITERAL;
    }

    rejectMaybeNull(left, node.getLeft(), op, node.getLocation());
    rejectMaybeNull(right, node.getRight(), op, node.getLocation());

    std::vector<std::shared_ptr<const FalxType>> results;

    for (const auto& leftM : membersOf(left)) {
        for (const auto& rightM : membersOf(right)) {
            if (const auto tmp = std::vector{leftM, rightM}; std::ranges::find(tmp, ANY) != tmp.end()) {
                results.push_back(std::ranges::find(COMPARISONS, op) != COMPARISONS.end()? BOOLEAN_LITERAL : ANY);
                continue;
            }

            const auto result = binaryResult(op, leftM, rightM);
            if (results.empty()) {
                throw std::runtime_error{"Wrong operand"};
            }
            results.push_back(*result);
        }
    }

    return makeUnion(results);
}


void ExpressionChecker::rejectMaybeNull(const std::shared_ptr<const FalxType> &type, const std::shared_ptr<Expression> &operand, const TokenKind &_operator, const SourceLocation &location) {
    if (const auto* ty = dynamic_cast<const UnionType*>(type.get());
         ty && std::ranges::any_of(ty->getMembers(), [](const auto& member) { return *member == *NULL_LITERAL; })){
        throw std::runtime_error{"Potentially null value!"};
    }
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_UnaryExpression(const UnaryExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) const {
    const auto& operand = this->inferValue(node.getExpression());
    const auto& op = node.getOperator().getTokenKind();

    if (dynamic_cast<const UnionType*>(operand.get()) && isNullable(operand)) {
        throw std::runtime_error{"Potentially null value!!"};
    }

    const std::vector<std::shared_ptr<const FalxType>> accepted = op == TokenKind::MINUS? NUMERIC
        : std::vector{BOOLEAN_LITERAL, STRING_LITERAL};

    std::vector<std::shared_ptr<const FalxType>> results;
    for (const auto& member : membersOf(operand)) {
        if (*member == *ANY) {
            results.push_back(op == TokenKind::MINUS? ANY : BOOLEAN_LITERAL);
        } else if (std::ranges::contains(accepted, member)) {
            results.push_back(op == TokenKind::MINUS? member : BOOLEAN_LITERAL);
        } else {
            throw std::runtime_error{"Cannot invert value " + member->toString() + "!"};
        }
    }

    return makeUnion(results);
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_LogicalExpression(const LogicalExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    const auto& op = node.getOperator().getTokenKind();
    const auto left = this->inferValue(node.getLeft());
    this->requireTruthy(left, node.getLeft(), OPERATOR_SYMBOLS.at(op), node.getLocation());

    const auto& [whenTrue, whenFalse] = this->factsFor(node.getLeft());

    const auto& facts = op == TokenKind::AND? whenTrue: whenFalse;

    const auto right = scope([&](TypeEnvironment&) { return inferValue(node.getRight()); }, facts);

    return makeUnion({left, right});
}

void ExpressionChecker::requireTruthy(const std::shared_ptr<const FalxType> &type, const std::shared_ptr<Expression> &operand, const std::string &_operator, const SourceLocation &location) {
    if (dynamic_cast<const UnionType*>(type.get()) && isNullable(type)) {
        throw std::runtime_error{"Potentially null value!!!"};
    }

    const auto tmp = std::vector{ANY, BOOLEAN_LITERAL, STRING_LITERAL};
    for (const auto& member : membersOf(type)) {
        if (!std::ranges::contains(tmp, member)) {
            throw std::runtime_error{"Cannot evaluate value"};
        }
    }
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_NullCoalesceExpression(const NullCoalesceExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) const {
    const auto& left = this->inferValue(node.getLeft(), expected);
    const auto& right = this->inferValue(node.getRight(), expected);

    if (*left == *ANY) {
        return ANY;
    }

    return makeUnion({nonNull(left), right});
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_OptionalChainExpression(const OptionalChainExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    this->m_chains.push_back(false);

    std::shared_ptr<const FalxType> inner;

    try {
        inner = infer(node.getChain(), std::move(expected));
    } catch (...) {
        this->m_chains.pop_back();
        throw;
    }

    const auto& shortCircuits = this->m_chains.back();
    this->m_chains.pop_back();

    return shortCircuits && *inner != *VOID? makeNullable(inner) : inner;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_GetExpression(const GetExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    std::shared_ptr<const FalxType> receiver = this->inferValue(node.getObject());
    receiver = this->nonNullReceiver(receiver, node, node.getObject(), "read '" + node.getProperty() + "'", node.isOptional());
    return this->memberType(receiver, node.getProperty(), node);
}

std::shared_ptr<const FalxType> ExpressionChecker::memberType(const std::shared_ptr<const FalxType> &receiver, const std::string &name, const std::variant<GetExpression, SetExpression> &node) {
    if (*receiver == *NEVER) {
        return NEVER;
    }

    std::vector<std::shared_ptr<const FalxType>> results;

    for (const auto& member : membersOf(receiver)) {
        results.push_back(this->_singleMember(member, name, node));
    }

    return makeUnion(results);
}

std::shared_ptr<const FalxType> ExpressionChecker::_singleMember(const std::shared_ptr<const FalxType> &receiver, const std::string &name, std::variant<GetExpression, SetExpression> node) {
    const auto& object = std::holds_alternative<GetExpression>(node)? std::get<GetExpression>(node).getObject() : std::get<SetExpression>(node).getObject();

    if (*receiver == *ANY) {
        return ANY;
    }

    std::vector<std::string> candidates;

    if (const auto* _struct = dynamic_cast<const StructType*>(receiver.get())) {
        const auto& info = _struct->getInfo();
        if (info.getFields().contains(name)) {
            return info.getFields().at(name).getType();
        }

        if (info.getMethods().contains(name)) {
            return std::make_shared<FunctionType>(info.getMethods().at(name));
        }

        for (const auto &fieldName: info.getFields() | std::views::keys) {
            candidates.push_back(fieldName);
        }

        for (const auto& methodName : info.getMethods() | std::views::keys) {
            candidates.push_back(methodName);
        }
    }
    else if (const auto* module = dynamic_cast<const ModuleType*>(receiver.get())) {
        if (module->getExports().contains(name)) {
            return module->getExports().at(name);
        }
        for (const auto& exportName : module->getExports() | std::views::keys) {
            candidates.push_back(exportName);
        }
    }
    else {
        if (const auto& member = builtinMember(receiver, name)) {
            return *member;
        }
        if (const auto* r = dynamic_cast<const MapType*>(receiver.get())) {
            return r->getValueType();
        }

        for (const auto& key : builtinMemberNames(receiver)) {
            candidates.push_back(key);
        }
    }

    std::string message = "'" + source(object) + "' ("+receiver->toString()+") as no property '" + name + "'";
    if (const auto close = getCloseMatches(name, candidates, 1); !close.empty()) {
        message += ", did you mean '" + close.front() + "'?";
    }
    throw std::runtime_error{"Cannot access property: " + message};
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_SetExpression(const SetExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    auto receiver = this->inferValue(node.getObject());
    receiver = nonNullReceiver(receiver, node, node.getObject(), "set '" + node.getProperty() + "'", false);

    auto value = this->inferValue(node.getValue());
    const auto& target = this->_settableMember(receiver, node);

    if (node.getOperator()) {
        value = this->_combineForAssignment(node.getOperator()->getTokenKind(), target, value, node.getLocation());
    }

    requireAssignable(target, value, "property '" + node.getProperty() + "'", node.getValue()->getLocation());
    return value;
}

std::shared_ptr<const FalxType> ExpressionChecker::_settableMember(const std::shared_ptr<const FalxType> &receiver, const SetExpression &node) {
    std::vector<std::shared_ptr<const FalxType>> results;

    for (const auto& member : membersOf(receiver)) {
        if (*member == *ANY) {
            results.push_back(ANY);
        }
        else if (const auto* _struct = dynamic_cast<const StructType*>(member.get())) {
            const auto& field = _struct->getInfo().getFields().at(node.getProperty());
            results.push_back(field.getType());
        }
        else if (const auto* map = dynamic_cast<const MapType*>(member.get())) {
            results.push_back(map->getValueType());
        }
        else if (const auto* tup = dynamic_cast<const TupleType*>(member.get())) {
            throw std::runtime_error{"Cannot set immutable tuple"};
        }
        else {
            throw std::runtime_error{"Cannot set property!"};
        }
    }

    return makeUnion(results);
}

std::shared_ptr<const FalxType> ExpressionChecker::_combineForAssignment(const TokenKind &_operator, const std::shared_ptr<const FalxType> &current, const std::shared_ptr<const FalxType> &value, const SourceLocation &location) {
    std::vector<std::shared_ptr<const FalxType>> results;


    for (const auto& leftM : membersOf(current)) {
        for (const auto& rightM : membersOf(value)) {
            if (const auto tmp = std::vector{leftM, rightM}; std::ranges::contains(tmp, ANY)) {
                results.push_back(ANY);
                continue;
            }
            const auto& result = binaryResult(_operator, leftM, rightM);
            if (!result) {
                throw std::runtime_error{"Operator error!"};
            }
            results.push_back(*result);
        }
    }

    return makeUnion(results);
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_IndexExpression(const IndexExpression &node, std::optional<std::shared_ptr<const FalxType> > expected) {
    auto receiver = this->inferValue(node.getObject());
    receiver = nonNullReceiver(receiver, node, node.getObject(), "index", node.isOptional());
    const auto& index = this->inferValue(node.getIndex());

    if (*receiver == *NEVER) {
        return NEVER;
    }

    std::vector<std::shared_ptr<const FalxType>> results;

    for (const auto& member : membersOf(receiver)) {
        results.push_back(this->_indexInto(member, index, node));
    }

    return makeUnion(results);
}

std::shared_ptr<const FalxType> ExpressionChecker::_indexInto(const std::shared_ptr<const FalxType> &receiver, const std::shared_ptr<const FalxType> &index, const IndexExpression &node) {
    if (*receiver == *ANY) {
        return ANY;
    }

    if (const auto* map = dynamic_cast<const MapType*>(receiver.get())) {
        requireAssignable(
            map->getKeyType(),
            index,
            "map key",
            node.getIndex()->getLocation()
        );

        return makeNullable(map->getValueType());
    }

    const auto* arr = dynamic_cast<const ArrayType*>(receiver.get());
    const auto* tup = dynamic_cast<const TupleType*>(receiver.get());

    if ( *receiver == *STRING_LITERAL || arr || tup) {
        requireIndex(index, node);

        if (*receiver == *STRING_LITERAL) {
            return STRING_LITERAL;
        }

        if (arr) {
            return arr->getElementType();
        }

        if (
            const auto* num = dynamic_cast<const NumberLiteral*>(node.getIndex().get());
            num && std::holds_alternative<int>(num->getValue())
        ) {
            const auto position = std::get<int>(num->getValue());
            const auto size = static_cast<std::ptrdiff_t>(tup->getElementTypes().size());

            if (-size <= position && position < size) {
                return tup->getElementTypes().at(
                    static_cast<std::size_t>(position < 0 ? position + size : position)
                );
            }
        }

        return makeUnion(tup->getElementTypes());
    }

    throw std::runtime_error{"Object is not indexable!"};
}

void ExpressionChecker::requireIndex(const std::shared_ptr<const FalxType> &index, const std::variant<IndexExpression, IndexSetExpression> &node) {
    if (!isAssignable(INTEGER_LITERAL, index)) {
        throw std::runtime_error{"Index must be an int, but got '" + index->toString() + "'"};
    }
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_IndexSetExpression(const IndexSetExpression &node, const std::optional<std::shared_ptr<const FalxType> >& expected) {
    auto receiver = this->inferValue(node.getObject());
    receiver = nonNullReceiver(receiver, node, node.getObject(), "assign on index", false);

    const auto& index = this->inferValue(node.getIndex());
    auto value = this->inferValue(node.getValue());

    std::vector<std::shared_ptr<const FalxType>> targets;

    for (const auto& member : membersOf(receiver)) {
        if (*member == *ANY) {
            targets.push_back(ANY);
        }
        else if (const auto* arr = dynamic_cast<const ArrayType*>(member.get())) {
            requireIndex(index, node);
            targets.push_back(arr->getElementType());
        }
        else if (const auto* map = dynamic_cast<const MapType*>(member.get())) {
            requireAssignable(map->getKeyType(), index, "map key", node.getIndex()->getLocation());
            targets.push_back(map->getValueType());
        }
        else if (dynamic_cast<const TupleType*>(member.get())) {
            throw std::runtime_error{"Immutable tuple found!"};
        }
        else {
            throw std::runtime_error{"Object not indexable!"};
        }
    }

    const auto& target = makeUnion(targets);

    if (node.getOperator()) {
        value = _combineForAssignment(node.getOperator()->getTokenKind(), dynamic_cast<const MapType*>(receiver.get())? makeNullable(target) : target, value, node.getLocation());
    }

    requireAssignable(target, value, "element", node.getValue()->getLocation());
    return value;
}

std::shared_ptr<const FalxType> ExpressionChecker::expression_FunctionCallExpression(const FunctionCallExpression &node, const std::optional<std::shared_ptr<const FalxType> >& expected) {
    if (const auto& module = _requireCall(node)) return *module;

    auto callee = this->inferValue(node.getCallee());

    if (*callee == *ANY) {
        for (const auto& argument : node.getArguments()) {
            static_cast<void>(this->inferValue(argument));
        }
        return ANY;
    }

    callee = nonNullReceiver(callee, node, node.getCallee(), "call it", false);
    if (*callee == *NEVER) {
        for (const auto& argument : node.getArguments()) {
            static_cast<void>(this->inferValue(argument));
        }
        return NEVER;
    }

    std::vector<FunctionType> signatures;

    for (const auto& member : membersOf(callee)) {
        if (*member == *ANY) {
            continue;
        }
        if (const auto* fun = dynamic_cast<const FunctionType*>(member.get())) {
            signatures.push_back(*fun);
        }
        throw std::runtime_error{"Object not callable!"};
    }

    if (signatures.empty()) {
        for (const auto& argument : node.getArguments()) {
            static_cast<void>(this->inferValue(argument));
        }
        return ANY;
    }

    if (signatures.size() > 1) {
        signatures = {_commonSignature(signatures, node)};
    }

    return _checkCall(signatures.front(), node);
}

FunctionType ExpressionChecker::_commonSignature(const std::vector<FunctionType> &signatures, const FunctionCallExpression &node) {
    if (signatures.empty()) {
        throw std::invalid_argument{"Expected at least one signature"};
    }

    const auto& first = signatures.front();

    for (std::size_t i = 1; i < signatures.size(); ++i) {
        if (const auto& other = signatures[i];
            other.getParameterTypes() != first.getParameterTypes() ||
            other.getMinArguments() != first.getMinArguments()
        ) {
            throw std::runtime_error{"Object not callable!!"};
        }
    }

    std::vector<std::shared_ptr<const FalxType>> returnTypes;

    for (const auto& signature : signatures) {
        returnTypes.push_back(signature.getReturnType());
    }

    return FunctionType{first.getParameterTypes(), makeUnion(returnTypes), first.getRequired(), first.getParameterNames()};
}

std::shared_ptr<const FalxType> ExpressionChecker::_checkCall(const FunctionType &signature, const FunctionCallExpression &node) const {
    if (const auto& count = node.getArguments().size(); !(signature.getMinArguments() < count && signature.getMaxArguments() > count)) {
        throw std::runtime_error{"Wrong number of arguments!"};
    }

    for (std::size_t i = 0; i < node.getArguments().size(); ++i) {
        const auto& arg = node.getArguments()[i];
        const auto& parameter = signature.getParameterTypes().at(i);
        const auto& actual = this->inferValue(arg, parameter);

        const auto& name = i < signature.getParameterNames().size() ? signature.getParameterNames().at(i) : "";

        requireAssignable(parameter, actual, name, node.getLocation());
    }

    return signature.getReturnType();
}

std::optional<std::shared_ptr<const FalxType> > ExpressionChecker::_requireCall(const FunctionCallExpression &node) const {
    const auto& callee = node.getCallee();
    if (const auto* req = dynamic_cast<const VariableExpression*>(callee.get()); req && req->getName() == "require") {
        return nullptr;
    }

    const auto& symbol = this->m_environment.lookup("require");
    if (!symbol || symbol.value()->getKind() != "builtin") {
        return nullptr;
    }

    if (node.getArguments().size() != 1) {
        throw std::runtime_error{"Wrong number of arguments!"};
    }

    const auto& argument = node.getArguments()[0];
    requireAssignable(REQUIRE_FUNCTION.getParameterTypes().front(), this->inferValue(argument), "argument 1", argument->getLocation());

    if (const auto* str = dynamic_cast<const StringLiteral*>(argument.get())) {
        return loadModule(str->getValue(), node.getLocation());
    }

    return ANY;
}

std::shared_ptr<const FalxType> ExpressionChecker::loadModule(const std::string &name, const SourceLocation &location) {
    throw std::runtime_error{"Not implemented!"};
}
