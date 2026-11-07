//
// Created by SharkBlocks00 on 10/31/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include <functional>
#include <set>

#include "falxtype/FalxType.h"
#include "../ast/types/expressions/ArrayTypeExpression.h"
#include "../ast/types/expressions/NamedTypeExpression.h"

const std::map<std::string, std::shared_ptr<const FalxType>> BUILTIN_TYPES{
    {"int", INTEGER_LITERAL},
    {"float", FLOAT_LITERAL},
    {"bool", BOOLEAN_LITERAL},
    {"String", STRING_LITERAL},
    {"string", STRING_LITERAL},
    {"null", NULL_LITERAL},
    {"any", ANY},
    {"void", VOID},
    {"unit", VOID},
    {"Exception", EXCEPTION},
    {"File", FILE_OBJECT},
};

using GenericTypeFactory = std::function<std::shared_ptr<const FalxType>(
    const std::vector<std::shared_ptr<const FalxType>>&
)>;

const std::map<std::string, std::pair<int, GenericTypeFactory>> GENERIC_TYPES{
    {"Map",
        {
            2,
            [](const auto& types) -> std::shared_ptr<const FalxType> {
                return std::make_shared<MapType>(types[0], types[1]);
            }
        }
    }
};


class TypeResolver {
public:
    using FindStruct = std::function<std::shared_ptr<const StructType>(const std::string&)>;
    using StructNames = std::function<std::vector<std::string>()>;
private:
    FindStruct m_findStruct;
    StructNames m_structNames;
    std::set<std::string> m_typeVariables;

    [[nodiscard]] std::shared_ptr<const FalxType> _resolve(const TypeExpressionPtr& expression);
    [[nodiscard]] std::shared_ptr<const FalxType> _operand(const TypeExpressionPtr &expression);
    [[nodiscard]] std::shared_ptr<const FalxType> _resolveNamed(NamedTypeExpression expression,
        std::string& name, std::vector<TypeExpressionPtr> arguments
    );
public:
    explicit TypeResolver(
        FindStruct findStruct = [](const std::string&) { return nullptr; } ,
        StructNames structNames = [] { return std::vector<std::string>{}; },
        std::vector<std::string> typeVariables = {});

    [[nodiscard]] std::shared_ptr<const FalxType> resolve(TypeExpressionPtr expression, const bool& allowVoid = false);


};