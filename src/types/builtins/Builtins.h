//
// Created by SharkBlocks00 on 11/6/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include "../../lexer/Lexer.h"
#include "../../parser/Parser.h"
#include "../falxtype/FalxType.h"
#include "../TypeResolver.h"


const std::shared_ptr<const FalxType>& parseType(const std::string& text, const std::vector<std::string>& variables={});
std::optional<std::shared_ptr<const FalxType>>& builtinMember(const std::shared_ptr<const FalxType>& receiver, const std::string& name);
std::vector<std::string>& builtinMemberNames(const std::shared_ptr<const FalxType>& receiver);
std::optional<const ModuleType>& internalModule(const std::string& name);

using Signature = std::variant<std::string, std::tuple<std::string, int>>;

class SignatureTable {
private:
    std::map<std::string, Signature> m_entries;
    std::vector<std::string> m_variables;
    mutable std::map<std::string, std::shared_ptr<const FalxType>> m_parsed{};

    [[nodiscard]] bool contains(const std::string& name) const;
public:
    explicit SignatureTable() = default;
    explicit SignatureTable(std::map<std::string, Signature> entries, std::vector<std::string> variables = {});

    [[nodiscard]] std::vector<std::string> names() const;
    [[nodiscard]] std::optional<std::shared_ptr<const FalxType>> get(const std::string& name) const;
    [[nodiscard]] std::map<std::string, std::shared_ptr<const FalxType>> asDict() const;
};

const auto STRING_MEMBERS = SignatureTable{
    {
        {"length", "int"},
        {"toUpper", "() -> String"},
        {"toLower", "() -> String"},
        {"trim", "() -> String"},
        {"toInt", "() -> int"},
        {"toReal", "() -> float"},
        {"contains", "(String) -> bool"},
        {"startsWith", "(String) -> bool"},
        {"endsWith", "(String) -> bool"},
        {"replace", "(String, String) -> String"},
        {"split", "(String) -> String[]"},
    }
};

const auto ARRAY_MEMBERS = SignatureTable{
    {
        {"size", "int"},
        {"isEmpty", "() -> bool"},
        {"contains", "(T) -> bool"},
        {"copy", "() -> T[]"},
        {"push", "(T) -> T[]"},
        {"pop", "() -> T?"},
        {"insert", "(int, T) -> T[]"},
        {"remove", "(int) -> T"},
        {"reverse", "() -> T[]"},
        {"sort", "() -> T[]"},
        {"clear", "() -> void"},
    }, {"T"},
};

const auto MAP_MEMBERS = SignatureTable{
    {
        {"size", "int"},
        {"isEmpty", "() -> bool"},
        {"contains", "(K) -> bool"},
        {"copy", "() -> Map<K, V>"},
        {"keys", "() -> K[]"},
        {"values", "() -> V[]"},
        {"remove", "(K) -> Map<K, V>"},
        {"clear", "() -> void"},
        {"entries", "() -> any[][]"}
    }, {"K", "V"}
};

const auto TUPLE_MEMBERS = SignatureTable{
    {
        {"size", "int",}
    }
};

const auto GLOBAL_FUNCTIONS = SignatureTable{
    {
        {"output", std::make_tuple("(any) -> void", 0)},
        {"typeof", std::string("(any) -> String")},
        {"assert", std::make_tuple("(any, any) -> void", 1)},
        {"request", std::make_tuple("(any) -> String", 0)},
    }, {}
};

const auto REQUIRE_FUNCTION = FunctionType{
    {STRING_LITERAL},
    parseType("any")
};

const std::map<std::string, SignatureTable> INTERNAL_MODULES{
        {
            {
                "math",
                SignatureTable{
                    {
                        {"pi", "float"},
                        {"e", "float"},
                        {"tau", "float"},
                        {"phi", "float"},
                        {"sqrt2", "float"},
                        {"sqrt3", "float"},
                        {"ln2", "float"},
                        {"ln10", "float"},
                        {"log2e", "float"},
                        {"log10e", "float"},
                        {"epsilon", "float"},
                        {"floor", "(float) -> int"},
                        {"ceil", "(float) -> int"},
                        {"trunc", "(float) -> int"},
                        {"round", "(float) -> int"},
                        {"sqrt", "(float) -> float"},
                    }
                }
            },
            {
                "random",
                SignatureTable{
                    {
                        {"random", "() -> float"},
                        {"random_int", "(int, int) -> int"},
                        {"seed", "((float | String)?) -> void"},
                        {"get_state", "() -> any"},
                        {"set_state", "(any) -> any"},
                    }
                }
            },
            {
                "time",
                SignatureTable{
                    {
                        {"time", "() -> float"},
                        {"monotonic", "() -> float"},
                        {"sleep", "(float) -> void"},
                        {"utc_now", "() -> Map<String, int>"},
                        {"local_now", "() -> Map<String, int>"},
                    }
                }
            },
            {
                "fio",
                SignatureTable{
                    {
                        {"open", "(String, String) -> File"},
                        {"close", "(File) -> void"},
                        {"read", std::make_tuple("(File, int) -> String", 1)},
                        {"write", "(File, any) -> int"}
                    }
                }
            }
        }
};