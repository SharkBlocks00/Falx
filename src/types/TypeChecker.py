from src.ast.Expression import Expression
from src.ast.expressions.literals.ArrayLiteral import ArrayLiteral
from src.ast.expressions.literals.BooleanLiteral import BooleanLiteral
from src.ast.expressions.literals.MapLiteral import MapLiteral
from src.ast.expressions.literals.NullLiteral import NullLiteral
from src.ast.expressions.literals.NumberLiteral import NumberLiteral
from src.ast.expressions.literals.StringLiteral import StringLiteral
from src.ast.expressions.literals.TupleLiteral import TupleLiteral
from src.ast.Statement import Statement

from src.types.FalxType import (
    ANY,
    BOOL,
    FLOAT,
    INT,
    NULL,
    STRING,
    ArrayType,
    FalxType,
    MapType,
    TupleType,
    make_union,
)
from src.types.TypeEnvironment import TypeEnvironment
from src.types.TypeErrors import TypeCheckError
from src.types.TypeRelations import is_assignable
from src.types.TypeResolver import TypeResolver


class TypeChecker:
    def __init__(self, resolver: TypeResolver | None = None) -> None:
        self.resolver = resolver or TypeResolver()
        self.environment = TypeEnvironment()

        self._expression_handlers = {
            BooleanLiteral: self._infer_boolean,
            NumberLiteral: self._infer_number,
            StringLiteral: self._infer_string,
            NullLiteral: self._infer_null,
            ArrayLiteral: self._infer_array,
            MapLiteral: self._infer_map,
            TupleLiteral: self._infer_tuple,
        }

    def check(self, statements: list[Statement]) -> None:
        for statement in statements:
            self.check_statement(statement)

    def check_statement(self, statement: Statement) -> None:
        if hasattr(statement, "initializer") and hasattr(statement, "name"):
            self._check_variable_declaration(statement)
            return

        raise TypeCheckError(
            f"Type checking is not implemented for "
            f"{type(statement).__name__}.",
            getattr(statement, "location", None),
        )

    def _check_variable_declaration(self, statement: Statement) -> None:
        name = statement.name
        initializer = statement.initializer
        location = statement.location
        annotation = getattr(statement, "type_annotation", None)

        actual_type = self.infer_expression(initializer)

        if annotation is not None:
            declared_type = self.resolver.resolve(annotation)

            if not is_assignable(declared_type, actual_type):
                raise TypeCheckError(
                    f"Cannot assign '{actual_type}' to variable "
                    f"'{name}' of type '{declared_type}'.",
                    location,
                )
        else:
            declared_type = actual_type

        try:
            self.environment.define(name, declared_type)
        except ValueError as error:
            raise TypeCheckError(str(error), location) from error

    def infer_expression(self, expression: Expression) -> FalxType:
        for expression_class in type(expression).__mro__:
            handler = self._expression_handlers.get(expression_class)

            if handler is not None:
                return handler(expression)

        raise TypeCheckError(
            f"Type inference is not implemented for "
            f"{type(expression).__name__}.",
            expression.location,
        )

    @staticmethod
    def _infer_boolean(expression: BooleanLiteral) -> FalxType:
        return BOOL

    @staticmethod
    def _infer_number(expression: NumberLiteral) -> FalxType:
        if isinstance(expression.value, int):
            return INT
        return FLOAT

    @staticmethod
    def _infer_string(expression: StringLiteral) -> FalxType:
        return STRING

    @staticmethod
    def _infer_null(expression: NullLiteral) -> FalxType:
        return NULL

    def _infer_array(self, expression: ArrayLiteral) -> FalxType:
        if not expression._elements:
            return ArrayType(ANY)

        element_types = [
            self.infer_expression(element)
            for element in expression._elements
        ]

        return ArrayType(make_union(*element_types))

    def _infer_map(self, expression: MapLiteral) -> FalxType:
        if not expression.values:
            return MapType(STRING, ANY)

        value_types = [
            self.infer_expression(value)
            for value in expression.values.values()
        ]

        return MapType(STRING, make_union(*value_types))

    def _infer_tuple(self, expression: TupleLiteral) -> FalxType:
        element_types = tuple(
            self.infer_expression(element)
            for element in expression.elements
        )

        return TupleType(element_types)
