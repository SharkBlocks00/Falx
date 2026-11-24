//
// Created by SharkBlocks00 on 11/11/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once

#include <filesystem>

#include "../ast/statements/BlockStatement.h"
#include "../ast/statements/BreakStatement.h"
#include "../ast/statements/ContinueStatement.h"
#include "../ast/statements/DestructureDeclaration.h"
#include "../ast/statements/ExpressionStatement.h"
#include "../ast/statements/ForeachStatement.h"
#include "../ast/statements/FunctionDeclaration.h"
#include "../ast/statements/IfStatement.h"
#include "../ast/statements/ReturnStatement.h"
#include "../ast/statements/StructDeclaration.h"
#include "../ast/statements/VariableDeclaration.h"
#include "../ast/statements/WhileStatement.h"
#include "environment/TypeEnvironment.h"
#include "expressions/ExpressionChecker.hpp"
#include "resolver/TypeResolver.h"

std::shared_ptr<TypeEnvironment> makeBuiltinScope();

struct FunctionContext {
    std::string name;
    std::optional<std::shared_ptr<const FalxType>> declaredReturn;
    std::vector<std::shared_ptr<const FalxType>> returnTypes{};
    bool hasBareReturn{false};
};

struct ModuleCache {
    std::unordered_map<std::filesystem::path, ModuleType> modules{};

    std::vector<std::filesystem::path> loading{};
};


class TypeChecker : public ExpressionChecker {
private:
    std::optional<std::filesystem::path> m_projectDir;
    std::optional<std::filesystem::path> m_currentFile;
    std::shared_ptr<ModuleCache> m_cache;
    bool m_isModule{false};
    std::filesystem::path m_stdlibDir{
        std::filesystem::weakly_canonical(std::filesystem::path(__FILE__)).parent_path().parent_path() / "modules" / "stdlib"
    };
    std::shared_ptr<TypeEnvironment> builtins = makeBuiltinScope();
    std::shared_ptr<TypeEnvironment> m_topScope;

    std::vector<FunctionContext> m_functions{};
    std::map<std::string, std::shared_ptr<const FalxType>> m_exports{};
    std::map<std::string, StructType> m_exportedStructs{};
    std::unordered_map<const StructDeclaration*, StructType> m_preparedStructs{};

    std::optional<StructType> iFindStruct(const std::string& name) const;
    std::optional<std::string> iOrigin() const;

    void iDefine(const std::string& name, const std::shared_ptr<const FalxType> &type, bool _mutable, const std::string& kind, const SourceLocation& location) const;
    void iExport(bool exported, const std::string& name, const std::shared_ptr<const FalxType>& type);

    static void iForgetAssigned(TypeEnvironment &scope, const std::set<std::string>& names);

    static void iRequireCondition(const std::shared_ptr<const FalxType>& type, const std::shared_ptr<Expression>& condition, const SourceLocation& location);
    static void iDefineIn(const std::shared_ptr<TypeEnvironment>& scope, const std::string& name, const std::shared_ptr<const FalxType>& type, const std::string& kind, const SourceLocation& location);
    void iDeclareStructMembers(const StructDeclaration& node, StructInfo& info);

    static std::map<std::string, std::optional<std::shared_ptr<const FalxType>>> iState(const std::set<std::string>& names, const TypeEnvironment& scope);

    static std::shared_ptr<const FalxType> iElementType(const std::shared_ptr<const FalxType>& iterable, const ForeachStatement& node);
    std::shared_ptr<const FalxType> iParameterType(const ParameterDefinition& parameter);
    std::shared_ptr<const FalxType> iFieldType(const FieldDefinition& field);

    FunctionType iCheckFunction(const FunctionDeclaration& node, bool define);
    FunctionType iConstructorType(const StructDeclaration& node, const StructType& _struct);

    std::filesystem::path iResolveUserModule(const std::string& name) const;
    ModuleType iLoadFile(const std::string& name, const std::filesystem::path& path, const SourceLocation& location);
public:
    explicit TypeChecker(const std::optional<std::filesystem::path>& projectDir = std::nullopt, const std::optional<std::filesystem::path>& currentFile = std::nullopt, std::shared_ptr<ModuleCache> cache = nullptr, const bool isModule = false)
        : m_projectDir(projectDir ? std::filesystem::weakly_canonical(*projectDir) : std::filesystem::weakly_canonical(std::filesystem::current_path())), m_currentFile(currentFile), m_cache(cache ? std::move(cache) : std::make_shared<ModuleCache>()), m_isModule(isModule) {
        m_environment = m_isModule ? std::make_shared<TypeEnvironment>(builtins, false) : builtins;
        m_topScope = m_environment;
    }

    void check(const std::vector<std::shared_ptr<Statement>>& statements);
    void defineBuiltin(const std::string& name, const std::shared_ptr<const FalxType>& type) const;
    void checkBlock(const std::vector<std::shared_ptr<Statement>>& statements);
    void checkStatement(const std::shared_ptr<Statement>& statement);

    TypeResolver resolver();
    std::shared_ptr<const FalxType> resolveType(const TypeExpressionPtr& annotation, bool allowVoid = false);
    std::shared_ptr<const FalxType> loadModule(const std::string& name, const SourceLocation& location) override;

    void prepareBlock(const std::vector<std::shared_ptr<Statement>>& statements);
    void statement_ExpressionStatement(const ExpressionStatement& node);
    void statement_BreakStatement(const BreakStatement& node);
    void statement_ContinueStatement(const ContinueStatement& node);
    void statement_BlockStatement(const BlockStatement& node);
    void statement_VariableDeclaration(const VariableDeclaration& node);
    void statement_DestructureDeclaration(const DestructureDeclaration& node);
    void statement_IfStatement(const IfStatement& node);
    void statement_WhileStatement(const WhileStatement& node);
    void statement_ForeachStatement(const ForeachStatement& node);
    void statement_FunctionDeclaration(const FunctionDeclaration& node);
    void statement_ReturnStatement(const ReturnStatement& node);
    void statement_StructDeclaration(const StructDeclaration& node);
};