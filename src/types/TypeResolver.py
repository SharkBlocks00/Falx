from src.types.FalxType import ANY, BOOL, FLOAT, INT, NULL, STRING, UNIT, VOID, ArrayType, FalxType, NamedType, NullableType, make_nullable, make_union
from src.types.TypeErrors import TypeCheckError

class TypeResolver:
    PRIMITIVES: dict[str, FalxType] = {
        "int": INT,
        "float": FLOAT,
        "bool": BOOL,
        "String": STRING,
        "string": STRING,
        "unit": UNIT,
        "void": VOID,
        "null": NULL,
        "any": ANY,
    }

    def __init__(self, known_types: set[str] | None = None,) -> None:
        self.known_types = set(known_types or ())

    def register_type(self, name: str) -> None:
        self.known_types.add(name)

    def resolve(self, annotation: str) -> FalxType:
        annotation = annotation.strip()

        if not annotation:
            raise TypeCheckError("Expected a type annotation.")

        return self._parse_union(annotation)


    def _parse_union(self, text: str) -> FalxType:
        parts = self._split_top_level(text, "|")

        if len(parts) > 1:
            return make_union(*(self._parse_postfix(part) for part in parts))

        return self._parse_postfix(text)

    def _parse_postfix(self, text: str) -> FalxType:
        text = text.strip()

        if text.endswith("[]"):
            inner = self._parse_postfix(text[:-2])
            return ArrayType(inner)

        if text.endswith("?"):
            inner = self._parse_postfix(text[:-1])
            return make_nullable(inner)

        if text.startswith("(") and text.endswith(")"):
            return self._parse_union(text[1:-1])

        if text in self.PRIMITIVES:
            return self.PRIMITIVES[text]

        if text in self.known_types:
            return NamedType(text)

        raise TypeCheckError(f"Unknown type '{text}'.")


    @staticmethod
    def _split_top_level(text: str, seperator: str,) -> list[str]:
        parts: list[str] = []
        depth: int = 0
        start: int = 0

        for i, c in enumerate(text):
            if c in "(<[":
                depth += 1
            elif c in ")>]":
                depth -= 1
            elif c == seperator and depth == 0:
                parts.append(text[start:i].strip())
                start = i + 1

        parts.append(text[start:].strip())

        if any(not part for part in parts):
            raise TypeCheckError(f"Invalid type expression '{text}'")

        return parts