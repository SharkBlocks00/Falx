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

const std::shared_ptr<const FalxType>& parseType(const std::string& text, const std::vector<std::string>& variables) {
    const auto expression = Parser{Lexer{text, "<builtin>"}.lex()}.parseTypeAnnotation();
    // return TypeResolver{variables}.resolve(expressioon, true);
    return nullptr;
}
