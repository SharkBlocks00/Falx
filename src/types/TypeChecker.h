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
    std::optional<ModuleCache> m_cache;
    bool m_isModule{false};
    std::filesystem::path m_stdlibDir{
        std::filesystem::canonical(__FILE__).parent_path().parent_path() / "modules" / "stdlib"
    };
    std::shared_ptr<TypeEnvironment> builtins = makeBuiltinScope();

    std::shared_ptr<TypeEnvironment> m_environment{m_isModule ? builtins->child() : builtins };
    std::shared_ptr<TypeEnvironment> m_topScope = m_environment;

    std::vector<bool> m_chains{};
    std::vector<FunctionContext> m_functions{};
    std::map<std::string, std::shared_ptr<const FalxType>> m_exports{};
    std::map<std::string, StructType> m_exportedStructs{};
    std::unordered_map<const StructDeclaration*, StructType> m_preparedStructs{};

    std::optional<StructType> iFindStruct(const std::string& name) const;
    std::optional<std::string> iOrigin() const;

    void iDefine(const std::string& name, const std::shared_ptr<const FalxType> &type, bool _mutable, const std::string& kind, const SourceLocation& location) const;
    void iExport(const bool& exported, const std::string& name, std::shared_ptr<const FalxType> type);

    static void iForgetAssigned(TypeEnvironment &scope, const std::set<std::string>& names);

    static void iRequireCondition(const std::shared_ptr<const FalxType>& type, const std::shared_ptr<Expression>& condition, const SourceLocation& location);
    void iDefineIn(const std::shared_ptr<TypeEnvironment>& scope, const std::string& name, std::shared_ptr<const FalxType> type, const std::string& kind, const SourceLocation& location);
    void iDeclareStructMembers(const StructDeclaration& node, const StructInfo& info);

    static std::map<std::string, std::optional<std::shared_ptr<const FalxType>>> iState(const std::set<std::string>& names, TypeEnvironment scope);

    static std::shared_ptr<const FalxType> iElementType(const std::shared_ptr<const FalxType>& iterable, const ForeachStatement& node);
    std::shared_ptr<const FalxType> iParameterType(const ParameterDefinition& parameter);
    std::shared_ptr<const FalxType> iFieldType(const FieldDefinition& field);

    FunctionType iCheckFunction(const FunctionDeclaration& node, bool define);
    FunctionType iConstructorType(const StructDeclaration& node, const StructType& _struct);

    std::filesystem::path iResolveUserModule(const std::string& name);
    ModuleType iLoadFile(const std::string& name, const std::filesystem::path& path, const SourceLocation& location);
public:
    explicit TypeChecker(const std::optional<std::filesystem::path>& projectDir = std::nullopt, const std::optional<std::filesystem::path>& currentFile = std::nullopt, const std::optional<ModuleCache> &cache = std::nullopt, const bool isModule = false)
        : m_projectDir(projectDir? projectDir : std::filesystem::canonical(std::filesystem::current_path())), m_currentFile(currentFile), m_cache(cache? cache : ModuleCache{}), m_isModule(isModule) {}

    void check(const std::vector<std::shared_ptr<Statement>>& statements);
    void defineBuiltin(const std::string& name, const std::shared_ptr<const FalxType>& type) const;
    void checkBlock(const std::vector<std::shared_ptr<Statement>>& statements);
    void checkStatement(const std::shared_ptr<Statement>& statement);

    TypeResolver resolver();
    std::shared_ptr<const FalxType> resolveType(const TypeExpressionPtr& annotation, bool allowVoid = false);
    std::shared_ptr<const FalxType> loadModule(const std::string& name, const SourceLocation& location);

    void prepareBlock(const std::vector<std::shared_ptr<Statement>>& statements);
    void statement_ExpressionStatement(const ExpressionStatement& node) const;
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