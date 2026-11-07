//
// Created by SharkBlocks00 on 11/7/2026.
// Copyright (c) Sharx All rights reserved.
//

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Narrowing.hpp"

#include "../ast/Expression.h"
#include "../ast/expressions/BinaryExpression.h"
#include "../ast/expressions/LogicalExpression.h"
#include "../ast/expressions/UnaryExpression.h"
#include "../ast/expressions/VariableExpression.h"
#include "../ast/expressions/literals/NullLiteral.h"
#include "TypeRelations.hpp"
#include "falxtype/FalxType.h"



std::pair<Facts, Facts> conditionFacts(
    const std::shared_ptr<Expression>& condition,
    const TypeLookup& lookup
) {
    if (const auto* expr = dynamic_cast<const BinaryExpression*>(condition.get())) {
        return _nullComparison(expr, lookup);
    }

    if (const auto* expr = dynamic_cast<const UnaryExpression*>(condition.get());
        expr && expr->getOperator().getTokenKind() == TokenKind::BANG) {

        const auto [whenTrue, whenFalse] =
            conditionFacts(expr->getExpression(), lookup);

        return {whenFalse, whenTrue};
        }

    if (const auto* expr = dynamic_cast<const LogicalExpression*>(condition.get())) {
        const auto [leftTrue, leftFalse] =
            conditionFacts(expr->getLeft(), lookup);

        const auto [rightTrue, rightFalse] =
            conditionFacts(expr->getRight(), lookup);

        if (expr->getOperator().getTokenKind() == TokenKind::AND) {
            return {
                _both(leftTrue, rightTrue),
                _either(leftFalse, rightFalse)
            };
        }

        return {
            _either(leftTrue, rightTrue),
            _both(leftFalse, rightFalse)
        };
    }

    return {Facts{}, Facts{}};
}

std::pair<Facts, Facts> _nullComparison(const BinaryExpression* condition, const TypeLookup& lookup) {
    const auto _operator = condition->getOperator().getTokenKind();

    if (_operator != TokenKind::EQUAL_EQUAL && _operator != TokenKind::BANG_EQUAL) {
        return {Facts{}, Facts{}};
    }

    const auto* rightNull = dynamic_cast<const NullLiteral*>(condition->getRight().get());
    const auto* leftNull = dynamic_cast<const NullLiteral*>(condition->getLeft().get());

    const auto* leftVariable = dynamic_cast<const VariableExpression*>(condition->getLeft().get());
    const auto* rightVariable = dynamic_cast<const VariableExpression*>(condition->getRight().get());

    const VariableExpression* variable = nullptr;

    if (rightNull && leftVariable) {
        variable = leftVariable;
    } else if (leftNull && rightVariable) {
        variable = rightVariable;
    } else {
        return {Facts{}, Facts{}};
    }

    const auto current = lookup(variable->getName());

    if (!current || !isNullable(*current)) {
        return {Facts{}, Facts{}};
    }

    Facts isNull{
        {variable->getName(), NULL_LITERAL}
    };

    Facts notNull{
        {variable->getName(), nonNull(*current)}
    };

    return _operator == TokenKind::EQUAL_EQUAL? std::make_pair(isNull, notNull) : std::make_pair(notNull, isNull);
}

Facts _both(const Facts &first, const Facts &second) {
    auto result = std::map{first};

    for (const auto& [name, type] : second) {
        if (result.contains(name)) {
            std::vector<std::shared_ptr<const FalxType>> res;
            for (const auto& member : membersOf(result.at(name))) {
                if (isAssignable(type, member)) {
                    res.push_back(member);
                }
            }
            result[name] = makeUnion(res);
        } else {
            result[name] = type;
        }
    }

    return result;
}

Facts _either(const Facts &first, const Facts &second) {
    Facts result;

    for (const auto& [name, type] : first) {
        if (const auto it = second.find(name); it != second.end()) {
            result.emplace(name, makeUnion({type, it->second}));
        }
    }

    return result;
}