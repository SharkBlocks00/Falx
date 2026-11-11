//
// Created by SharkBlocks00 on 11/11/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <memory>
#include <set>
#include <vector>

#include "../../ast/ASTNode.h"
#include "../../ast/Expression.h"
#include "../../ast/Statement.h"

bool terminates(std::vector<std::shared_ptr<Statement>>& statements, bool loopJumps = false);
bool iTerminates(const std::shared_ptr<Statement>& statement, bool loopJumps);

bool containsBreak(const std::vector<std::shared_ptr<Statement>>& statements);

std::vector<ASTNode> children(const ASTNode& node);

template<typename T, std::enable_if_t<std::is_base_of_v<Statement, T> || std::is_base_of_v<Expression, T>, int> = 0>
std::vector<ASTNode> children(const std::shared_ptr<T>& node) {
    if (!node) return {};
    if constexpr (has_ast_children<T>::value) {
        return flattenASTChildren(*node);
    } else if constexpr (std::is_base_of_v<Statement, T>) {
        return children(ASTNode{std::static_pointer_cast<Statement>(node)});
    } else {
        return children(ASTNode{std::static_pointer_cast<Expression>(node)});
    }
}

template<typename T>
std::vector<ASTNode> iFlatten(const T& value) { return flattenASTChildren(value); }


std::vector<ASTNode> walk(const ASTNode& node);

template<typename T, std::enable_if_t<std::is_base_of_v<Statement, T> || std::is_base_of_v<Expression, T>, int> = 0>
std::vector<ASTNode> walk(const std::shared_ptr<T>& node) {
    if constexpr (std::is_base_of_v<Statement, T>) {
        return walk(ASTNode{std::static_pointer_cast<Statement>(node)});
    } else {
        return walk(ASTNode{std::static_pointer_cast<Expression>(node)});
    }
}

std::set<std::string> assignedNames(const std::vector<std::shared_ptr<Statement>>& statements);
std::set<std::string> declaredNames(const std::vector<std::shared_ptr<Statement>>& statements);
