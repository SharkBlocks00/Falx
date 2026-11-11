//
// Created by SharkBlocks00 on 11/11/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "StatementAnalysis.h"

#include <algorithm>

#include "../../ast/expressions/literals/BooleanLiteral.h"
#include "../../ast/expressions/AssignmentExpression.h"
#include "../../ast/expressions/BinaryExpression.h"
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
#include "../../ast/expressions/literals/ArrayLiteral.h"
#include "../../ast/expressions/literals/MapLiteral.h"
#include "../../ast/expressions/literals/TupleLiteral.h"
#include "../../ast/statements/BlockStatement.h"
#include "../../ast/statements/BreakStatement.h"
#include "../../ast/statements/ContinueStatement.h"
#include "../../ast/statements/IfStatement.h"
#include "../../ast/statements/DestructureDeclaration.h"
#include "../../ast/statements/ExpressionStatement.h"
#include "../../ast/statements/ForeachStatement.h"
#include "../../ast/statements/FunctionDeclaration.h"
#include "../../ast/statements/ReturnStatement.h"
#include "../../ast/statements/StructDeclaration.h"
#include "../../ast/statements/VariableDeclaration.h"
#include "../../ast/statements/WhileStatement.h"

template<typename Base, typename T, typename... Rest>
std::vector<ASTNode> childrenAs(const std::shared_ptr<Base>& node) {
    if (const auto concrete = std::dynamic_pointer_cast<const T>(node)) {
        return flattenASTChildren(*concrete);
    }
    if constexpr (sizeof...(Rest) > 0) {
        return childrenAs<Base, Rest...>(node);
    }
    return {};
}

bool terminates(std::vector<std::shared_ptr<Statement> > &statements, const bool loopJumps) {
    return std::any_of(statements.begin(), statements.end(),
    [&](const std::shared_ptr<Statement>& statement) {
        return iTerminates(statement, loopJumps);
    });
}

bool iTerminates(const std::shared_ptr<Statement>& statement, bool loopJumps) {
    const auto* returns = dynamic_cast<const ReturnStatement*>(statement.get());
    // TODO: add `throw` here:
    if (returns) {
        return true;
    }

    const auto* breaks = dynamic_cast<const BreakStatement*>(statement.get());
    const auto* continues = dynamic_cast<const ContinueStatement*>(statement.get());
    if (breaks || continues) {
        return loopJumps;
    }

    if (const auto* block = dynamic_cast<const BlockStatement*>(statement.get())) {
        return terminates(block->getStatements(), loopJumps);
    }

    if (const auto* ifS = dynamic_cast<const IfStatement*>(statement.get())) {
        return ifS->getElseBody() && terminates(ifS->getStatements(), loopJumps)
            && iTerminates(ifS->getElseBody(), loopJumps);
    }

    if (const auto* whileS = dynamic_cast<const WhileStatement*>(statement.get())) {
        const auto* isBool = dynamic_cast<const BooleanLiteral*>(whileS->getCondition().get());

        return isBool && isBool->getValue() && !containsBreak(whileS->getBody());
    }

    return false;
}

bool containsBreak(const std::vector<std::shared_ptr<Statement>>& statements) {
    for (const auto& statement : statements) {
        for (auto nodes = walk(std::variant<std::shared_ptr<Statement>, std::shared_ptr<Expression>>{statement}); const auto& node : nodes) {
            const bool isBreak = std::visit([]<typename T0>(const T0& ptr) {
                using T = std::decay_t<T0>::element_type;

                if constexpr (std::is_base_of_v<Statement, T>) {
                    return std::dynamic_pointer_cast<BreakStatement>(ptr) != nullptr;
                }

                return false;
            }, node);

            if (isBreak) {
                return true;
            }
        }
    }
    return false;
}

std::vector<ASTNode> children(const ASTNode& node) {
    return std::visit([]<typename T0>(const T0& pointer) {
        using T = typename std::decay_t<T0>::element_type;
        if (!pointer) return std::vector<ASTNode>{};

        if constexpr (std::is_base_of_v<Expression, T>) {
            return childrenAs<Expression,
                AssignmentExpression, BinaryExpression, DestructureAssignment,
                FunctionCallExpression, GetExpression, IndexExpression,
                IndexSetExpression, LogicalExpression, NullCoalesceExpression,
                OptionalChainExpression, SetExpression, UnaryExpression,
                ArrayLiteral, MapLiteral, TupleLiteral>(pointer);
        } else {
            return childrenAs<Statement,
                BlockStatement, DestructureDeclaration, ExpressionStatement,
                ForeachStatement, FunctionDeclaration, IfStatement,
                ReturnStatement, StructDeclaration, VariableDeclaration,
                WhileStatement>(pointer);
        }
    }, node);
}

std::vector<ASTNode> walk(const ASTNode& node) {
    std::vector<ASTNode> result;
    if (std::visit([](const auto& pointer) { return static_cast<bool>(pointer); }, node)) {
        result.push_back(node);
    }

    for (const auto& child : children(node)) {
        const auto descendants = walk(child);
        result.insert(result.end(), descendants.begin(), descendants.end());
    }
    return result;
}

std::set<std::string> assignedNames(const std::vector<std::shared_ptr<Statement> > &statements) {
    std::set<std::string> names;

    for (const auto& statement : statements) {
        for (const auto& node : walk(statement)) {
            std::visit([&](const auto& ptr) {
                if (const auto* assign = dynamic_cast<const AssignmentExpression*>(ptr.get())) {
                    names.insert(assign->getName());
                }
                else if (const auto* destructure = dynamic_cast<const DestructureAssignment*>(ptr.get())) {
                    for (const auto& target : destructure->getTargets()) {
                        names.insert(target.getName());
                    }
                }
            }, node);
        }
    }

    return names;
}

std::set<std::string> declaredNames(const std::vector<std::shared_ptr<Statement> > &statements) {
    std::set<std::string> names;

    for (const auto& statement : statements) {
        const auto* isVar = dynamic_cast<const VariableDeclaration*>(statement.get());
        const auto* isFun = dynamic_cast<const FunctionDeclaration*>(statement.get());
        const auto* isStr = dynamic_cast<const StructDeclaration*>(statement.get());

        if (isVar || isFun || isStr) {
            names.insert(isVar? isVar->getName() : isFun? isFun->getName() : isStr->getName());
        }
        else if (const auto* destructure = dynamic_cast<const DestructureDeclaration*>(statement.get())) {
            for (const auto& target : destructure->getTargets()) {
                names.insert(target.getName());
            }
        }
    }

    return names;
}
