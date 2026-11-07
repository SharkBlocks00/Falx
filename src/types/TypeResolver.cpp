//
// Created by SharkBlocks00 on 10/31/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "TypeResolver.h"

#include <ranges>
#include <utility>

#include "../ast/types/expressions/FunctionTypeExpression.h"
#include "../ast/types/expressions/NullableTypeExpression.h"
#include "../ast/types/expressions/TupleTypeExpression.h"
#include "../ast/types/expressions/UnionTypeExpression.h"
#include "../helpers/StringUtils.h"

TypeResolver::TypeResolver(FindStruct findStruct, StructNames structNames, std::vector<std::string> typeVariables)
    : m_findStruct(std::move(findStruct)), m_structNames(std::move(structNames)), m_typeVariables(typeVariables.begin(), typeVariables.end()) {}

std::shared_ptr<const FalxType> TypeResolver::resolve(TypeExpressionPtr expression, const bool &allowVoid) {
    const auto resolved = this->_resolve(std::move(expression));

    if (resolved == VOID && !allowVoid) {
        throw std::runtime_error{"'void' can only be used as a function's return type"};
    }

    return resolved;
}

std::shared_ptr<const FalxType> TypeResolver::_resolve(const TypeExpressionPtr& expression) {
    if (const auto* named = dynamic_cast<const NamedTypeExpression*>(expression.get())) {
        return _resolveNamed(*named, named->getName(), named->getArguments());
    }

    if (const auto* nullable = dynamic_cast<const NullableTypeExpression*>(expression.get())) {
        return makeNullable(_operand(nullable->getInner()));
    }

    if (const auto* array = dynamic_cast<const ArrayTypeExpression*>(expression.get())) {
        return std::make_shared<ArrayType>(_operand(array->getElement()));
    }

    if (const auto* unionExpr = dynamic_cast<const UnionTypeExpression*>(expression.get())) {
        std::vector<std::shared_ptr<const FalxType>> members;

        for (const auto& member : unionExpr->getMembers()) {
            members.push_back(_operand(member));
        }

        return makeUnion(members);
    }

    if (const auto* tupleExpr = dynamic_cast<const TupleTypeExpression*>(expression.get())) {
        std::vector<std::shared_ptr<const FalxType>> elements;

        for (const auto& element : tupleExpr->getElements()) {
            elements.push_back(_operand(element));
        }

        return std::make_shared<TupleType>(elements);
    }

    if (const auto* functionExpr = dynamic_cast<const FunctionTypeExpression*>(expression.get())) {
        std::vector<std::shared_ptr<const FalxType>> parameters;

        for (const auto& param : functionExpr->getParameters()) {
            parameters.push_back(_operand(param));
        }

        return std::make_shared<FunctionType>(parameters, _resolve(functionExpr->getReturnType()));
    }

    throw std::runtime_error{"Unsupported type annotation '" + expression->toString() + '.'};
}

std::shared_ptr<const FalxType> TypeResolver::_operand(const TypeExpressionPtr &expression) {
    return this->resolve(expression);
}

std::shared_ptr<const FalxType> TypeResolver::_resolveNamed(NamedTypeExpression expression, std::string &name, std::vector<TypeExpressionPtr> arguments) {
    if (GENERIC_TYPES.contains(name)) {
        const auto& [expected, build] = GENERIC_TYPES.at(name);

        if (arguments.size() != expected) {
            throw std::runtime_error{"Type '" + name + "' needs " + std::to_string(expected)
                + " type arguments, but got" + std::to_string(arguments.size()) + " arguments"
            };
        }

        std::vector<std::shared_ptr<const FalxType>> args;

        for (const auto& argument : arguments) {
            args.push_back(_operand(argument));
        }

        return build(args);
    }

    if (!arguments.empty()) {
        throw std::runtime_error{"Type '" + name + "' does not take type arguments"};
    }

    if (this->m_typeVariables.contains(name)) {
        return std::make_shared<TypeVar>(name);
    }

    if (BUILTIN_TYPES.contains(name)) {
        return BUILTIN_TYPES.at(name);
    }

    if (const auto& _struct = m_findStruct(name)) {
        return _struct;
    }

    std::vector<std::string> candidates;

    for (const auto &title: BUILTIN_TYPES | std::views::keys) {
        candidates.push_back(title);
    }
    for (const auto& title  : GENERIC_TYPES | std::views::keys) {
        candidates.push_back(title);
    }

    const auto structNames = m_structNames();
    candidates.insert(candidates.end(), structNames.begin(), structNames.end());

    const auto close = getCloseMatches(name, candidates, 1);

    std::string message = "Unknown type '" + name + "'";

    if (!close.empty()) {
        message += ", did you mean '" + close[0] +"'?";
    }
    throw std::runtime_error{message};
}
