from src.types.FalxType import ANY, NULL, FalxType, NullableType, UnionType

def is_assignable(expected: FalxType, actual: FalxType) -> bool:
    if expected == actual:
        return True

    if expected == ANY or actual == ANY:
        return True

    if actual == NULL:
        return isinstance(expected, NullableType)

    if isinstance(expected, NullableType):
        if isinstance(actual, NullableType):
            return is_assignable(expected.inner, actual.inner)

        return is_assignable(expected.inner, actual)

    if isinstance(actual, NullableType):
        return False

    if isinstance(expected, UnionType):
        return any(is_assignable(member, actual) for member in expected.members)

    if isinstance(actual, UnionType):
        return all(is_assignable(expected, member) for member in actual.members)

    return False