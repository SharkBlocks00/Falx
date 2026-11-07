//
// Created by SharkBlocks00 on 11/6/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "Builtins.h"

#include <ranges>

// SignatureTable

SignatureTable::SignatureTable(std::map<std::string, Signature> entries, std::vector<std::string> variables)
    : m_entries(std::move(entries)), m_variables(std::move(variables)) {}

bool SignatureTable::contains(const std::string &name) const {
    return m_entries.contains(name);
}

std::vector<std::string> SignatureTable::names() const {
    std::vector<std::string> names;

    std::transform(m_entries.begin(), m_entries.end(), std::back_inserter(names),
        [](const std::pair<const std::string, Signature> &entry) { return entry.first; }
    );
    return names;
}

std::optional<std::shared_ptr<const FalxType> > SignatureTable::get(const std::string& name) const {
    if (!m_entries.contains(name)) {
        return nullptr;
    }

    if (!m_parsed.contains(name)) {
        const auto& entry = m_entries.at(name);
        std::string text;
        std::optional<int> required;

        if (const auto* signature = std::get_if<std::string>(&entry)) {
            text = *signature;
        } else {
            const auto& [s, count] = std::get<std::tuple<std::string, int>>(entry);

            text = s;
            required = count;
        }

        auto parsed = parseType(text, this->m_variables);

        if (required) {
            if (const auto* expr = dynamic_cast<const FunctionType*>(parsed.get())) {
                parsed = std::make_shared<FunctionType>(expr->getParameterTypes(), expr->getReturnType(), &*required);
            }
        }

        this->m_parsed.emplace(name, std::move(parsed));
    }

    return this->m_parsed.at(name);
}

std::map<std::string, std::shared_ptr<const FalxType> > SignatureTable::asDict() const {
    std::map<std::string, std::shared_ptr<const FalxType>> dict;

    for (const auto &name: m_entries | std::views::keys) {
        if (auto parsed = get(name)) {
            dict.emplace(name, *parsed);
        }
    }

    return dict;
}

std::shared_ptr<const FalxType> parseType(const std::string& text, const std::vector<std::string>& variables) {
    const auto expression = Parser{Lexer{text, "<builtin>"}.lex()}.parseTypeAnnotation();

    TypeResolver resolver{{}, {}, variables};
    return resolver.resolve(expression, true);
}

std::optional<std::shared_ptr<const FalxType>> builtinMember(const std::shared_ptr<const FalxType> &receiver,
                                                             const std::string &name) {
    if (*receiver == *STRING_LITERAL) {
        return STRING_MEMBERS.get(name);
    }

    if (const auto* array = dynamic_cast<const ArrayType*>(receiver.get())) {
        const auto member = ARRAY_MEMBERS.get(name);

        if (!member) return nullptr;
        return substitute(member.value(), {{"T", array->getElementType()}});
    }

    if (const auto* map = dynamic_cast<const MapType*>(receiver.get())) {
        const auto member = MAP_MEMBERS.get(name);

        if (!member) return nullptr;
        return substitute(member.value(), {{"K", map->getKeyType()}, {"V", map->getValueType()}});
    }

    if (const auto tup = dynamic_cast<const TupleType*>(receiver.get())) {
        return TUPLE_MEMBERS.get(name);
    }

    return nullptr;
}

std::vector<std::string> builtinMemberNames(const std::shared_ptr<const FalxType> &receiver) {
    if (*receiver == *STRING_LITERAL) {
        return STRING_MEMBERS.names();
    }

    if (dynamic_cast<const ArrayType*>(receiver.get())) {
        return ARRAY_MEMBERS.names();
    }

    if (dynamic_cast<const MapType*>(receiver.get())) {
        return MAP_MEMBERS.names();
    }

    if (dynamic_cast<const TupleType*>(receiver.get())) {
        return TUPLE_MEMBERS.names();
    }
    return {};
}

std::optional<ModuleType> internalModule(const std::string& name) {
    if (name == "exceptions") {
        const auto c = parseType("(any) -> Exception");

        if (const auto* con = dynamic_cast<const FunctionType*>(c.get())) {
            const auto constructor = std::make_shared<FunctionType>(
                con->getParameterTypes(),
                con->getReturnType(),
                nullptr
            );

            std::map<std::string, std::shared_ptr<const FalxType>> exports{
                    {"Exception", constructor}
            };

            return ModuleType{"exceptions", exports};
        }
    }

    const auto it = INTERNAL_MODULES.find(name);

    if (it == INTERNAL_MODULES.end())
        return std::nullopt;

    return ModuleType{name, it->second.asDict()};
}
