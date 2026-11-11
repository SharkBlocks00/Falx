//
// Created by SharkBlocks00 on 11/7/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include "../Narrowing.hpp"
#include "../../ast/Expression.h"
#include "../../ast/expressions/AssignmentExpression.h"
#include "../../ast/expressions/DestructureAssignment.h"
#include "../../ast/expressions/FunctionCallExpression.h"
#include "../../ast/expressions/GetExpression.h"
#include "../../ast/expressions/IndexExpression.h"
#include "../../ast/expressions/IndexSetExpression.h"
#include "../../ast/expressions/LogicalExpression.h"
#include "../../ast/expressions/NullCoalesceExpression.h"
#include "../../ast/expressions/OptionalChainExpression.h"
#include "../../ast/expressions/SetExpression.h"
#include "../../ast/expressions/UnaryExpression.h"
#include "../../ast/expressions/VariableExpression.h"
#include "../../ast/expressions/literals/ArrayLiteral.h"
#include "../../ast/expressions/literals/BooleanLiteral.h"
#include "../../ast/expressions/literals/MapLiteral.h"
#include "../../ast/expressions/literals/NullLiteral.h"
#include "../../ast/expressions/literals/NumberLiteral.h"
#include "../../ast/expressions/literals/StringLiteral.h"
#include "../../ast/expressions/literals/TupleLiteral.h"
#include "../../tokens/TokenKind.h"
#include "../environment/TypeEnvironment.h"
#include "../falxtype/FalxType.h"

const std::vector NUMERIC = {INTEGER_LITERAL, FLOAT_LITERAL};
constexpr std::array COMPARISONS = {TokenKind::GREATER, TokenKind::GREATER_EQUAL, TokenKind::LESS, TokenKind::LESS_EQUAL};
constexpr std::array EQUALITIES = {TokenKind::EQUAL_EQUAL, TokenKind::BANG_EQUAL};
const std::map<TokenKind, std::string> OPERATOR_SYMBOLS = {
    {TokenKind::PLUS, "+"},
    {TokenKind::MINUS, "-"},
    {TokenKind::STAR, "*"},
    {TokenKind::SLASH, "/"},
    {TokenKind::PERCENT, "%"},
    {TokenKind::GREATER, ">"},
    {TokenKind::GREATER_EQUAL, ">="},
    {TokenKind::LESS, "<"},
    {TokenKind::LESS_EQUAL, "<="},
    {TokenKind::AND, "&&"},
    {TokenKind::OR, "||"},
};

std::optional<std::shared_ptr<const FalxType>> binaryResult(const TokenKind& op, std::shared_ptr<const FalxType>& left, std::shared_ptr<const FalxType>& right);
std::shared_ptr<const FalxType> widen(std::shared_ptr<const FalxType> type, const bool& mut = true);
std::shared_ptr<const FalxType> flowType(std::shared_ptr<const FalxType> declared, std::shared_ptr<const FalxType> value);

std::shared_ptr<const FalxType> loadModule(const std::string& name, const SourceLocation& location);

class ExpressionChecker {
private:
    TypeEnvironment m_environment{};
    std::vector<bool> m_chains;

    static std::string source(const std::shared_ptr<Expression>& expression);

    template<typename T, typename Part>
    std::optional<std::shared_ptr<const FalxType>> hint(
        const std::optional<std::shared_ptr<const FalxType>>& expected,
        Part&& part
    ) const {
        if (!expected || !*expected) {
            return std::nullopt;
        }

        for (const auto& member : membersOf(*expected)) {
            if (const auto typed = std::dynamic_pointer_cast<const T>(member)) {
                return std::invoke(std::forward<Part>(part), *typed);
            }
        }

        return std::nullopt;
    }

    static std::shared_ptr<const FalxType> _singleMember(const std::shared_ptr<const FalxType>& receiver, const std::string& name, std::variant<GetExpression, SetExpression> node);
    static std::shared_ptr<const FalxType> _settableMember(const std::shared_ptr<const FalxType>& receiver, const SetExpression& node);
    static std::shared_ptr<const FalxType> _combineForAssignment(const TokenKind& _operator, const std::shared_ptr<const FalxType>& current, const std::shared_ptr<const FalxType>& value, const SourceLocation& location);

    static std::shared_ptr<const FalxType> _indexInto(const std::shared_ptr<const FalxType>& receiver, const std::shared_ptr<const FalxType>& index, const IndexExpression& node);

    static FunctionType _commonSignature(const std::vector<FunctionType>& signatures, const FunctionCallExpression& node);

    [[nodiscard]] std::shared_ptr<const FalxType> _checkCall(const FunctionType& signature, const FunctionCallExpression& node) const;
    [[nodiscard]] std::optional<std::shared_ptr<const FalxType>> _requireCall(const FunctionCallExpression& node) const;

    static std::shared_ptr<const FalxType> loadModule(const std::string& name, const SourceLocation& location);
public:
    [[nodiscard]] static std::shared_ptr<const FalxType> infer(const std::shared_ptr<Expression>& expression, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    [[nodiscard]] std::shared_ptr<const FalxType> inferValue(const std::shared_ptr<Expression>& expression, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt) const;

    template<typename Callback> auto scope(Callback&& callback, std::optional<Facts> facts = std::nullopt,
        bool functionBoundary = false
    ) -> std::invoke_result_t<Callback, TypeEnvironment&>;

    std::pair<Facts, Facts> factsFor(const std::shared_ptr<Expression>& condition);

    static std::string describe(const std::shared_ptr<Expression>& expression);

    [[nodiscard]] std::shared_ptr<const FalxType> nonNullReceiver(const std::shared_ptr<const FalxType>& type, const Expression& node, const std::shared_ptr<Expression>& receiver, const std::string& action, const bool& optional);

    // Literals
    static std::shared_ptr<const FalxType> expression_NumberLiteral(const NumberLiteral& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);

    static std::shared_ptr<const FalxType> expression_StringLiteral(const StringLiteral& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);

    static std::shared_ptr<const FalxType> expression_BooleanLiteral(const BooleanLiteral& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);

    static std::shared_ptr<const FalxType> expression_NullLiteral(const NullLiteral& node, const std::optional<std::shared_ptr<const FalxType>> &expected = std::nullopt);
    [[nodiscard]] std::shared_ptr<const FalxType> expression_ArrayLiteral(const ArrayLiteral& node, const std::optional<std::shared_ptr<const FalxType>> &expected = std::nullopt) const;
    [[nodiscard]] std::shared_ptr<const FalxType> expression_MapLiteral(const MapLiteral& node, const std::optional<std::shared_ptr<const FalxType>> &expected = std::nullopt) const;
    [[nodiscard]] std::shared_ptr<const FalxType> expression_TupleLiteral(const TupleLiteral& node, const std::optional<std::shared_ptr<const FalxType>> &expected = std::nullopt) const;

    // Variables
    [[nodiscard]] std::shared_ptr<const FalxType> expression_VariableExpression(const VariableExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt) const;
    std::shared_ptr<const FalxType> expression_AssignmentExpression(const AssignmentExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    std::shared_ptr<const FalxType> expression_DestructureAssignmentExpression(const DestructureAssignment& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);

    static std::vector<std::shared_ptr<const FalxType>> unpackTuple(const std::shared_ptr<const FalxType>& value, const int& count, const SourceLocation& location);

    // Operators
    [[nodiscard]] std::shared_ptr<const FalxType> expression_BinaryExpression(const BinaryExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt) const;
    [[nodiscard]] std::shared_ptr<const FalxType> expression_UnaryExpression(const UnaryExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt) const;
    std::shared_ptr<const FalxType> expression_LogicalExpression(const LogicalExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    [[nodiscard]] std::shared_ptr<const FalxType> expression_NullCoalesceExpression(const NullCoalesceExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt) const;

    // Members
    std::shared_ptr<const FalxType> expression_OptionalChainExpression(const OptionalChainExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    std::shared_ptr<const FalxType> expression_GetExpression(const GetExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    std::shared_ptr<const FalxType> memberType(const std::shared_ptr<const FalxType>& receiver, const std::string& name, const std::variant<GetExpression, SetExpression>& node);
    std::shared_ptr<const FalxType> expression_SetExpression(const SetExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    std::shared_ptr<const FalxType> expression_IndexExpression(const IndexExpression& node, std::optional<std::shared_ptr<const FalxType>> expected = std::nullopt);
    std::shared_ptr<const FalxType> expression_IndexSetExpression(const IndexSetExpression& node, const std::optional<std::shared_ptr<const FalxType>>& expected = std::nullopt);

    // Calls
    std::shared_ptr<const FalxType> expression_FunctionCallExpression(const FunctionCallExpression& node, const std::optional<std::shared_ptr<const FalxType>>& expected = std::nullopt);

    static void requireIndex(const std::shared_ptr<const FalxType>& index, const std::variant<IndexExpression, IndexSetExpression>& node);
    static void requireTruthy(const std::shared_ptr<const FalxType>& type, const std::shared_ptr<Expression>& operand, const std::string& _operator, const SourceLocation& location);

    static void rejectMaybeNull(const std::shared_ptr<const FalxType>& type, const std::shared_ptr<Expression>& operand, const TokenKind& _operator, const SourceLocation& location);

    static void requireAssignable(const std::shared_ptr<const FalxType>& expected, const std::shared_ptr<const FalxType>& actual, const std::string& what, std::optional<SourceLocation> location = std::nullopt);

};