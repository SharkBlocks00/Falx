//
// Created by SharkBlocks00 on 11/3/2026.
// Copyright (c) Sharx All rights reserved.
//

#include "FalxType.h"

#include <utility>

// Literals

const std::shared_ptr<const FalxType> NULL_LITERAL = std::make_shared<PrimitiveType>("null");
const std::shared_ptr<const FalxType> INTEGER_LITERAL = std::make_shared<PrimitiveType>("int");
const std::shared_ptr<const FalxType> FLOAT_LITERAL = std::make_shared<PrimitiveType>("float");
const std::shared_ptr<const FalxType> BOOLEAN_LITERAL = std::make_shared<PrimitiveType>("bool");
const std::shared_ptr<const FalxType> STRING_LITERAL = std::make_shared<PrimitiveType>("String");
const std::shared_ptr<const FalxType> ANY = std::make_shared<PrimitiveType>("any");
const std::shared_ptr<const FalxType> VOID = std::make_shared<PrimitiveType>("void");
const std::shared_ptr<const FalxType> NEVER = std::make_shared<PrimitiveType>("never");
const std::shared_ptr<const FalxType> EXCEPTION = std::make_shared<PrimitiveType>("Exception");
const std::shared_ptr<const FalxType> FILE_OBJECT = std::make_shared<PrimitiveType>("File");

[[nodiscard]] bool isSimpleNullable(const UnionType& _union) {
    return _union.getMembers().size() == 2 && (std::find(_union.getMembers().begin(), _union.getMembers().end(), NULL_LITERAL) != _union.getMembers().end());
}

[[nodiscard]] std::string wrap(const std::shared_ptr<const FalxType>& type) {
    if (dynamic_cast<const FunctionType*>(type.get())) {
        return "(" + type->toString() + ")";
    }

    if (const auto* unionType = dynamic_cast<const UnionType*>(type.get())) {
        if (!isSimpleNullable(*unionType)) {
            return "(" + unionType->toString() + ")";
        }
    }

    return type->toString();
}


// PrimitiveType

PrimitiveType::PrimitiveType(std::string name) : m_name(std::move(name)) {}
std::string PrimitiveType::toString() const { return m_name; }
std::string PrimitiveType::getName() const { return m_name; }

bool PrimitiveType::equals(const FalxType &other) const {
    if (const auto* expr = dynamic_cast<const PrimitiveType*>(&other)) {
        return m_name == expr->getName();
    }
    return false;
}

// UnionType

UnionType::UnionType(const std::vector<std::shared_ptr<const FalxType> > &members) : m_members(members) {}
std::vector<std::shared_ptr<const FalxType> > UnionType::getMembers() const { return m_members; }

bool UnionType::equals(const FalxType& other) const {
    const auto* expr = dynamic_cast<const UnionType*>(&other);

    if (expr == nullptr) {
        return false;
    }

    const auto& otherMembers = expr->getMembers();

    if (m_members.size() != otherMembers.size()) {
        return false;
    }

    for (const auto& member : m_members) {
        const bool found = std::any_of(
            otherMembers.begin(),
            otherMembers.end(),
            [&member](const auto& candidate) {
                return member->equals(*candidate);
            });

        if (!found) {
            return false;
        }
    }
    return true;
}

std::string UnionType::toString() const {
    std::vector<std::shared_ptr<const FalxType>> others;

    for (const auto& member : m_members) {
        if (!member->equals(*NULL_LITERAL)) {
            others.push_back(member);
        }
    }

    std::ostringstream result;

    if (others.size() == m_members.size()) {
        for (std::size_t i = 0; i < others.size(); ++i) {
            if (i > 0) {
                result << " | ";
            }

            if (dynamic_cast<const FunctionType*>(others[i].get())) {
                result << "(" << others[i]->toString() << ")";
            } else {
                result << others[i]->toString();
            }
        }

        return result.str();
    }

    if (others.size() == 1) {
        return wrap(others[0]) + "?";
    }

    for (std::size_t i = 0; i < others.size(); ++i) {
        if (i > 0) {
            result << " | ";
        }

        result << others[i]->toString();
    }

    return "(" + result.str() + ")?";
}

// ArrayType

ArrayType::ArrayType(const std::shared_ptr<const FalxType> &elementType) : m_elementType(elementType) {}
const std::shared_ptr<const FalxType>& ArrayType::getElementType() const { return m_elementType; }

std::string ArrayType::toString() const { return wrap(m_elementType) + "[]"; }
bool ArrayType::equals(const FalxType& other) const {
    if (const auto* otherType = dynamic_cast<const ArrayType*>(&other)) {
        return m_elementType->equals(*otherType->getElementType());
    }
    return false;
}

// MapType

MapType::MapType(const std::shared_ptr<const FalxType> &keyType, const std::shared_ptr<const FalxType> &valueType)
    : m_keyType(keyType), m_valueType(valueType) {}

const std::shared_ptr<const FalxType> &MapType::getKeyType() const { return m_keyType; }
const std::shared_ptr<const FalxType> &MapType::getValueType() const { return m_valueType; }

bool MapType::equals(const FalxType& other) const {
    if (const auto* otherType = dynamic_cast<const MapType*>(&other)) {
        return m_keyType->equals(*otherType->getKeyType()) && m_valueType->equals(*otherType->getValueType());
    }
    return false;
}

std::string MapType::toString() const {
    return "Map<" + m_keyType->toString() + ", " +  m_valueType->toString() + ">";
}

// TupleType

TupleType::TupleType(const std::vector<std::shared_ptr<const FalxType> > &elementTypes)
    : m_elementTypes(elementTypes) {}

const std::vector<std::shared_ptr<const FalxType> > &TupleType::getElementTypes() const { return m_elementTypes; }

bool TupleType::equals(const FalxType& other) const {
    if (const auto* otherType = dynamic_cast<const TupleType*>(&other)) {
        return m_elementTypes == otherType->getElementTypes();
    }
    return false;
}

std::string TupleType::toString() const {
    std::ostringstream result;

    result << "(";

    for (std::size_t i =0; i < m_elementTypes.size(); ++i) {
        if (i > 0) {
            result << ", ";
        }
        result << m_elementTypes[i]->toString();
    }

    if (m_elementTypes.size() == 1) {
        result << ",";
    }
    result << ")";
    return result.str();
}

// FunctionType

FunctionType::FunctionType(const std::vector<std::shared_ptr<const FalxType> > &parameterTypes, const std::shared_ptr<const FalxType> &returnType, int *required, std::vector<std::string> parameterNames)
    : m_parameterTypes(parameterTypes), m_returnType(returnType), m_required(required), m_parameterNames(std::move(parameterNames)) {}

std::size_t FunctionType::getMaxArguments() const {
    return m_parameterTypes.size();
}

std::size_t FunctionType::getMinArguments() const {
    if (!m_required) {
        return m_parameterTypes.size();
    }
    return reinterpret_cast<std::size_t>(m_required);
}

void FunctionType::setRequired(int *required) {
    m_required = required;
}

std::size_t FunctionType::getParameterCount() const {
    return m_parameterTypes.size();
}

std::vector<std::string> FunctionType::getParameterNames() const {
    return m_parameterNames;
}

int *FunctionType::getRequired() const {
    return m_required;
}

const std::vector<std::shared_ptr<const FalxType> > &FunctionType::getParameterTypes() const {
    return m_parameterTypes;
}

const std::shared_ptr<const FalxType> &FunctionType::getReturnType() const {
    return m_returnType;
}

std::string FunctionType::toString() const {
    std::ostringstream result;

    result << "(";

    for (std::size_t i = 0; i < m_parameterTypes.size(); ++i) {
        if (i > 0) {
            result << ", ";
        }
        result << m_parameterTypes[i]->toString();
    }

    result << ") -> " << m_returnType->toString();

    return result.str();
}

bool FunctionType::equals(const FalxType &other) const {
    if (const auto* otherType = dynamic_cast<const FunctionType*>(&other)) {
        return m_returnType->equals(*otherType->getReturnType())
            && m_required == otherType->m_required
            && m_parameterTypes == otherType->m_parameterTypes;
    }

    return false;
}

// FieldInfo

FieldInfo::FieldInfo(const std::shared_ptr<const FalxType> &type, const bool hasDefault)
    : m_type(type), m_hasDefault(hasDefault) {}

const std::shared_ptr<const FalxType> &FieldInfo::getType() const {
    return m_type;
}

bool FieldInfo::isDefault() const {
    return m_hasDefault;
}

std::string FieldInfo::toString() const {
    return m_type->toString() + (m_hasDefault? "true" : "false");
}

bool FieldInfo::equals(const FalxType &other) const {
    if (const auto* otherType = dynamic_cast<const FieldInfo*>(&other)) {
        return m_type == otherType->m_type && m_hasDefault == otherType->m_hasDefault;
    }
    return false;
}

// StructInfo

StructInfo::StructInfo(std::string name, std::string *origin)
    : m_name(std::move(name)), m_origin(origin) {}

const std::map<std::string, FieldInfo> &StructInfo::getFields() const {
    return m_fields;
}

const std::string &StructInfo::getName() const {
    return m_name;
}

const std::map<std::string, FunctionType> &StructInfo::getMethods() const {
    return m_methods;
}

const std::string &StructInfo::getOrigin() const {
    return *m_origin;
}

void StructInfo::setOrigin(std::string *origin) {
    m_origin = origin;
}

std::string StructInfo::toString() const {
    std::ostringstream result;

    result << "(" << m_name << " : " << m_origin << " = ";

    std::size_t i = 0;
    for (const auto&[fst, snd] : m_methods) {
        if (i > 0) {
            result << ", ";
        }
        result << fst << " : " << snd.toString();
        ++i;
    }
    i=0;
    result << " && ";

    for (const auto&[fst, snd] : m_fields) {
        if (i > 0) {
            result << ", ";
        }
        result << fst << " : " << snd.toString();
        ++i;
    }
    result << ")";
    return result.str();
}

bool StructInfo::equals(const FalxType &other) const {
    if (const auto* otherType = dynamic_cast<const StructInfo*>(&other) ) {
        return m_name == otherType->m_name && m_origin == otherType->m_origin
            && m_methods == otherType->m_methods && m_fields == otherType->m_fields;
    }
    return false;
}


// StructType

StructType::StructType(StructInfo info)
    : m_info(std::move(info)) {}

const StructInfo &StructType::getInfo() const {
    return m_info;
}

std::string StructType::toString() const {
    return m_info.getName();
}

bool StructType::equals(const FalxType &other) const {
    if (const auto* otherType = dynamic_cast<const StructType*>(&other)) {
        return m_info == otherType->m_info;
    }
    return false;
}

// ModuleType

ModuleType::ModuleType(std::string name, const std::map<std::string, std::shared_ptr<const FalxType> >& exports, const std::map<std::string, const StructType>& types)
    : m_name(std::move(name)), m_exports(exports), m_types(types) {}

const std::map<std::string, std::shared_ptr<const FalxType> > &ModuleType::getExports() const {
    return m_exports;
}

const std::map<std::string, const StructType> &ModuleType::getTypes() const {
    return m_types;
}

const std::string &ModuleType::getName() const {
    return m_name;
}

std::string ModuleType::toString() const {
    return "module '" + m_name + "'";
}

bool ModuleType::equals(const FalxType &other) const {
    if (const auto* otherType = dynamic_cast<const ModuleType*>(&other)) {
        return m_name == otherType->m_name
            && m_exports == otherType->m_exports && m_types == otherType->m_types;
    }
    return false;
}

// TypeVar

TypeVar::TypeVar(std::string name) : m_name(std::move(name)) {}

const std::string &TypeVar::getName() const {
    return m_name;
}

std::string TypeVar::toString() const {
    return m_name;
}

bool TypeVar::equals(const FalxType &other) const {
    if (const auto* otherType = dynamic_cast<const TypeVar*>(&other)) {
        return m_name == otherType->m_name;
    }
    return false;
}


std::shared_ptr<const FalxType> makeUnion(const std::vector<std::shared_ptr<const FalxType>>& members) {
    std::vector<std::shared_ptr<const FalxType>> flattened;

    for (const auto& member : members) {
        const auto* unionType = dynamic_cast<const UnionType*>(member.get());

        auto process = [&](const auto& candidate) {
            if (candidate == ANY) {
                return true;
            }

            if (candidate != NEVER &&
                std::find(flattened.begin(), flattened.end(), candidate) == flattened.end()) {
                flattened.push_back(candidate);
            }

            return false;
        };

        if (unionType) {
            for (const auto& candidate : unionType->getMembers()) {
                if (process(candidate)) return ANY;
            }
        } else {
            if (process(member)) return ANY;
        }
    }

    if (flattened.empty()) {
        return NEVER;
    }

    if (flattened.size() == 1) {
        return flattened.front();
    }

    return std::make_shared<UnionType>(flattened);
}

std::shared_ptr<const FalxType> makeNullable(const std::shared_ptr<const FalxType>& type) {
    /// T? (T | null)
    return makeUnion(std::vector{type, ANY});
}

std::vector<std::shared_ptr<const FalxType>> membersOf(const std::shared_ptr<const FalxType>& type) {
    /// The alternatives a type is made of
    if (const auto* tmp = dynamic_cast<const UnionType*>(type.get())) {
        return tmp->getMembers();
    }
    return std::vector{type};
}

bool isNullable(const std::shared_ptr<const FalxType> &type) {
    /// True if null appears in this type
    const auto tmp = membersOf(type);
    return std::ranges::find(tmp, NULL_LITERAL) != tmp.end();
}

std::shared_ptr<const FalxType> nonNull(const std::shared_ptr<const FalxType>& type) {
    /// T for T?
    const auto tmp = membersOf(type);
    std::vector<std::shared_ptr<const FalxType>> members;

    for (const auto& member : membersOf(type)) {
        if (member != NULL_LITERAL) {
            members.push_back(member);
        }
    }
    return makeUnion(members);
}

std::shared_ptr<const FalxType> substitute(const std::shared_ptr<const FalxType>& type, const std::map<std::string, std::shared_ptr<const FalxType>>& bindings) {
    if (const auto* tmp = dynamic_cast<const TypeVar*>(type.get())) {
        const auto it = bindings.find(tmp->getName());

        return it != bindings.end() ? it->second : type;
    }

    if (const auto* tmp = dynamic_cast<const ArrayType*>(type.get())) {
        return std::make_shared<ArrayType>(substitute(tmp->getElementType(), bindings));
    }

    if (const auto* tmp = dynamic_cast<const MapType*>(type.get())) {
        return std::make_shared<MapType>(
            substitute(tmp->getKeyType(), bindings),
            substitute(tmp->getValueType(), bindings)
        );
    }

    if (const auto* tmp = dynamic_cast<const TupleType*>(type.get())) {
        std::vector<std::shared_ptr<const FalxType>> members;

        for (const auto& element : tmp->getElementTypes()) {
            members.push_back(element);
        }

        return std::make_shared<TupleType>(members);
    }

    if (const auto* tmp = dynamic_cast<const UnionType*>(type.get())) {
        std::vector<std::shared_ptr<const FalxType>> members;

        for (const auto& member : tmp->getMembers()) {
            members.push_back(substitute(member, bindings));
        }

        return makeUnion(members);
    }

    if (const auto* tmp = dynamic_cast<const FunctionType*>(type.get())) {
        std::vector<std::shared_ptr<const FalxType>> params;

        for (const auto& param : tmp->getParameterTypes()) {
            params.push_back(substitute(param, bindings));
        }

        return std::make_shared<FunctionType>(params, substitute(tmp->getReturnType(), bindings), tmp->getRequired(), tmp->getParameterNames());
    }

    return type;
}