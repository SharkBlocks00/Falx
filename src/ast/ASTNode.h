// Common AST traversal primitives.
#pragma once

#include <concepts>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

class Statement;
class Expression;

using ASTNode = std::variant<std::shared_ptr<Statement>, std::shared_ptr<Expression>>;

template<typename T, typename = void>
struct has_ast_children : std::false_type {};

template<typename T>
struct has_ast_children<T, std::void_t<decltype(std::declval<const T&>().astChildren())>>
    : std::true_type {};

inline void appendASTChildren(std::vector<ASTNode>& result, const ASTNode& value) {
    if (std::visit([](const auto& pointer) { return static_cast<bool>(pointer); }, value)) result.push_back(value);
}

template<typename T, std::enable_if_t<std::is_base_of_v<Statement, T> || std::is_base_of_v<Expression, T>, int> = 0>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::shared_ptr<T>& value) {
    if (value) result.emplace_back(value);
}

template<typename T>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::optional<T>& value) {
    if (value) appendASTChildren(result, *value);
}

template<typename T>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::vector<T>& values) {
    for (const auto& value : values) appendASTChildren(result, value);
}

template<typename Key, typename Value>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::map<Key, Value>& values) {
    for (const auto& [_, value] : values) appendASTChildren(result, value);
}

template<typename Key, typename Value>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::unordered_map<Key, Value>& values) {
    for (const auto& [_, value] : values) appendASTChildren(result, value);
}

template<typename... T>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::variant<T...>& value) {
    std::visit([&result](const auto& item) { appendASTChildren(result, item); }, value);
}

template<typename... T>
inline void appendASTChildren(std::vector<ASTNode>& result, const std::tuple<T...>& value) {
    std::apply([&result](const auto&... item) { (appendASTChildren(result, item), ...); }, value);
}

template<typename T, std::enable_if_t<has_ast_children<T>::value, int> = 0>
inline void appendASTChildren(std::vector<ASTNode>& result, const T& value) {
    std::apply([&result](const auto&... child) {
        (appendASTChildren(result, child), ...);
    }, value.astChildren());
}

template<typename T, std::enable_if_t<!has_ast_children<T>::value, int> = 0>
inline void appendASTChildren(std::vector<ASTNode>&, const T&) {}

template<typename... T>
inline std::vector<ASTNode> flattenASTChildren(const T&... values) {
    std::vector<ASTNode> result;
    (appendASTChildren(result, values), ...);
    return result;
}
