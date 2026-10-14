
from dataclasses import dataclass


class FalxType:
    __slots__ = ()


@dataclass(frozen=True)
class PrimitiveType(FalxType):
    name: str

    def __str__(self) -> str:
        return self.name


@dataclass(frozen=True)
class NamedType(FalxType):
    name: str

    def __str__(self) -> str:
        return self.name


@dataclass(frozen=True)
class NullableType(FalxType):
    inner: FalxType

    def __str__(self) -> str:
        if isinstance(self.inner, UnionType):
            return f"({self.inner})?"
        return f"{self.inner}?"


@dataclass(frozen=True)
class UnionType(FalxType):
    members: tuple[FalxType, ...]

    def __str__(self) -> str:
        return " | ".join(str(member) for member in self.members)


@dataclass(frozen=True)
class ArrayType(FalxType):
    element_type: FalxType

    def __str__(self) -> str:
        return f"{self.element_type}[]"


@dataclass(frozen=True)
class FunctionType(FalxType):
    parameter_types: tuple[FalxType, ...]
    return_type: FalxType
    thrown_exceptions: tuple[FalxType, ...] = ()

    def __str__(self) -> str:
        parameters = ", ".join(map(str, self.parameter_types))
        result = f"({parameters}) -> {self.return_type}"

        if self.thrown_exceptions:
            exceptions = " | ".join(map(str, self.thrown_exceptions))
            result += f"!{exceptions}"

        return result

@dataclass(frozen=True)
class MapType(FalxType):
    key_type: FalxType
    value_type: FalxType

    def __str__(self) -> str:
        return f"Map{self.key_type}, {self.value_type}"

@dataclass(frozen=True)
class TupleType(FalxType):
    element_types: tuple[FalxType, ...]

    def __str__(self) -> str:
        elements = ", ".join(map(str, self.element_types))
        return f"({elements})"


INT = PrimitiveType("int")
FLOAT = PrimitiveType("float")
BOOL = PrimitiveType("bool")
STRING = PrimitiveType("String")
UNIT = PrimitiveType("unit")
NULL = PrimitiveType("null")
ANY = PrimitiveType("any")


def make_nullable(inner: FalxType) -> FalxType:
    if isinstance(inner, NullableType):
        return inner

    if inner == NULL:
        return NULL

    return NullableType(inner)


def make_union(*members: FalxType) -> FalxType:
    flattened: list[FalxType] = []

    for member in members:
        candidates = (
            member.members
            if isinstance(member, UnionType)
            else (member,)
        )

        for candidate in candidates:
            if candidate not in flattened:
                flattened.append(candidate)

    if not flattened:
        raise ValueError("A union must contain at least one type.")

    if len(flattened) == 1:
        return flattened[0]

    return UnionType(tuple(flattened))
