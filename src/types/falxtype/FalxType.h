//
// Created by SharkBlocks00 on 10/31/2026.
// Copyright (c) Sharx All rights reserved.
//

#pragma once
#include <any>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <sstream>



class FalxType {
private:
    std::vector<std::any> m_slots;

public:
    virtual ~FalxType() = default;

    [[nodiscard]] virtual std::string toString() const = 0;
    [[nodiscard]] virtual bool equals(const FalxType& other) const = 0;

    bool operator==(const FalxType& other) const {
        return equals(other);
    }

    bool operator!=(const FalxType& other) const {
        return !equals(other);
    }

};


class PrimitiveType final : public FalxType {
private:
    std::string m_name;

public:
    explicit PrimitiveType(std::string name);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] bool equals(const FalxType& other) const override;
};


class UnionType final : public FalxType {
private:
    std::vector<std::shared_ptr<const FalxType>> m_members;

public:
    explicit UnionType(const std::vector<std::shared_ptr<const FalxType>>& members);

    [[nodiscard]] std::vector<std::shared_ptr<const FalxType>> getMembers() const;
    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] bool equals(const FalxType& other) const override;
};

class ArrayType final : public FalxType {
private:
    std::shared_ptr<const FalxType> m_elementType;
public:
    explicit ArrayType(const std::shared_ptr<const FalxType>& elementType);

    [[nodiscard]] const std::shared_ptr<const FalxType>& getElementType() const;
    [[nodiscard]] std::string toString() const override;

    [[nodiscard]] bool equals(const FalxType& other) const override;
};

class MapType final : public FalxType {
private:
    std::shared_ptr<const FalxType> m_keyType;
    std::shared_ptr<const FalxType> m_valueType;

public:
    explicit MapType(const std::shared_ptr<const FalxType>& keyType, const std::shared_ptr<const FalxType>& valueType);
    [[nodiscard]] const std::shared_ptr<const FalxType>& getKeyType() const;
    [[nodiscard]] const std::shared_ptr<const FalxType>& getValueType() const;

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool equals(const FalxType& other) const override;
};


class TupleType final : public FalxType {
private:
    std::vector<std::shared_ptr<const FalxType>> m_elementTypes;
public:
    explicit TupleType(const std::vector<std::shared_ptr<const FalxType>>& elementTypes);
    [[nodiscard]] const std::vector<std::shared_ptr<const FalxType>>& getElementTypes() const;

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool equals(const FalxType& other) const override;
};


class FunctionType final : public FalxType {
private:
    std::vector<std::shared_ptr<const FalxType>> m_parameterTypes;
    std::shared_ptr<const FalxType> m_returnType;
    int* m_required{nullptr};
    std::vector<std::string> m_parameterNames;
public:
    explicit FunctionType(const std::vector<std::shared_ptr<const FalxType>>& parameterTypes);

    [[nodiscard]] const std::vector<std::shared_ptr<const FalxType>>& getParameterTypes() const;
    [[nodiscard]] const std::shared_ptr<const FalxType>& getReturnType() const;
    [[nodiscard]] std::size_t getParameterCount() const;
    [[nodiscard]] std::vector<std::string> getParameterNames() const;

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool equals(const FalxType& other) const override;

    [[nodiscard]] std::size_t getMinArguments() const;
    [[nodiscard]] std::size_t getMaxArguments() const;

    void setRequired(int* required);
};


class FieldInfo final : public FalxType {
private:
    std::vector<std::any> m_slots{"type", "has_default"};

    std::shared_ptr<const FalxType> m_type;
    bool m_hasDefault;
public:
    explicit FieldInfo(const std::shared_ptr<const FalxType>& type, bool hasDefault);

    [[nodiscard]] const std::shared_ptr<const FalxType>& getType() const;
    [[nodiscard]] bool isDefault() const;

    [[nodiscard]] bool equals(const FalxType& other) const override;
    [[nodiscard]] std::string toString() const override;
};


class StructInfo : public FalxType {
private:
    std::string m_name;
    std::string* m_origin{nullptr};
    std::map<std::string, FieldInfo> m_fields;
    std::map<std::string, FunctionType> m_methods;
public:
    explicit StructInfo(std::string  name, std::string* origin);

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] const std::string& getOrigin() const;
    [[nodiscard]] const std::map<std::string, FieldInfo>& getFields() const;
    [[nodiscard]] const std::map<std::string, FunctionType>& getMethods() const;

    [[nodiscard]] bool equals(const FalxType& other) const override;
    [[nodiscard]] std::string toString() const override;

    void setOrigin(std::string* origin);
};


class StructType final : public FalxType {
private:
    StructInfo m_info;
public:
    explicit StructType(StructInfo  info);

    [[nodiscard]] const StructInfo& getInfo() const;
    [[nodiscard]] bool equals(const FalxType& other) const override;
    [[nodiscard]] std::string toString() const override;
};


class ModuleType final : public FalxType {
private:
    std::string m_name;
    std::map<std::string, std::shared_ptr<const FalxType>> m_exports;
    std::map<std::string, const StructType> m_types;
public:
    explicit ModuleType(std::string name, const std::map<std::string, std::shared_ptr<const FalxType>>& exports, const std::map<std::string, const StructType>& types);

    [[nodiscard]] const std::map<std::string, std::shared_ptr<const FalxType>>& getExports() const;
    [[nodiscard]] const std::map<std::string, const StructType>& getTypes() const;
    [[nodiscard]] const std::string& getName() const;

    [[nodiscard]] bool equals(const FalxType& other) const override;
    [[nodiscard]] std::string toString() const override;
};


class TypeVar final : public FalxType {
private:
    std::string m_name;
public:
    explicit TypeVar(std::string name);

    [[nodiscard]] const std::string& getName() const;

    [[nodiscard]] bool equals(const FalxType& other) const override;
    [[nodiscard]] std::string toString() const override;
};