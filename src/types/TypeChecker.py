from src.ast.Expression import Expression
from src.ast.expressions.literals.ArrayLiteral import ArrayLiteral
from src.ast.expressions.literals.BooleanLiteral import BooleanLiteral
from src.ast.expressions.literals.MapLiteral import MapLiteral
from src.ast.expressions.literals.NullLiteral import NullLiteral
from src.ast.expressions.literals.NumberLiteral import NumberLiteral
from src.ast.expressions.literals.StringLiteral import StringLiteral
from src.ast.expressions.literals.TupleLiteral import TupleLiteral
from src.ast.Statement import Statement

from src.ast.expressions.VariableExpression import VariableExpression
from src.ast.expressions.BinaryExpression import BinaryExpression
from src.ast.expressions.FunctionCallExpression import FunctionCallExpression
from src.ast.statements.ExpressionStatement import ExpressionStatement
from src.ast.statements.VariableDeclaration import VariableDeclaration
from src.ast.statements.FunctionDeclaration import FunctionDeclaration
from src.ast.statements.ReturnStatement import ReturnStatement
from src.ast.statements.IfStatement import IfStatement
from src.ast.statements.WhileStatement import WhileStatement
from src.ast.statements.ForeachStatement import ForeachStatement
from src.ast.statements.BlockStatement import BlockStatement
from src.ast.statements.BreakStatement import BreakStatement
from src.ast.statements.ContinueStatement import ContinueStatement

from src.tokens.TokenKind import TokenKind

from src.types.FalxType import ANY, BOOL, FLOAT, INT, NULL, STRING, VOID, ArrayType, FalxType, MapType, TupleType, UnionType, make_union, FunctionType
from src.types.TypeEnvironment import TypeEnvironment
from src.types.TypeErrors import TypeCheckError
from src.types.TypeRelations import is_assignable
from src.types.TypeResolver import TypeResolver


class TypeChecker:
    def __init__(self, resolver: TypeResolver | None = None) -> None:
        self.resolver = resolver or TypeResolver()
        self.environment = TypeEnvironment()

        self._expression_handlers = {
            VariableExpression: self._infer_variable,
            BinaryExpression: self._infer_binary,
            BooleanLiteral: self._infer_boolean,
            NumberLiteral: self._infer_number,
            StringLiteral: self._infer_string,
            NullLiteral: self._infer_null,
            ArrayLiteral: self._infer_array,
            MapLiteral: self._infer_map,
            TupleLiteral: self._infer_tuple,
            FunctionCallExpression: self._infer_function_call,
        }

    def check(self, statements: list[Statement]) -> None:
        for statement in statements:
            self.check_statement(statement)

    def check_statement(self, statement: Statement) -> None:
        if isinstance(statement, ExpressionStatement):
            self.infer_expression(statement.expression)
            return

        if isinstance(statement, VariableDeclaration):
            self._check_variable_declaration(statement)
            return

        if isinstance(statement, FunctionDeclaration):
            self._check_function_declaration(statement)
            return

        if isinstance(statement, ReturnStatement):
            if statement.value is not None:
                self.infer_expression(statement.value)
            return

        if isinstance(statement, IfStatement):
            self.infer_expression(statement.condition)
            for stmt in statement.body:
                self.check_statement(stmt)
            if statement.elseBody is not None:
                self.check_statement(statement.elseBody)
            return

        if isinstance(statement, WhileStatement):
            self.infer_expression(statement.condition)
            for stmt in statement.body:
                self.check_statement(stmt)
            return

        if isinstance(statement, ForeachStatement):
            iterable_type = self.infer_expression(statement.iterable)
            if isinstance(iterable_type, ArrayType):
                elem_type = iterable_type.element_type
            elif isinstance(iterable_type, MapType):
                elem_type = iterable_type.key_type
            elif isinstance(iterable_type, TupleType):
                elem_type = make_union(*iterable_type.element_types)
            else:
                elem_type = ANY
            loop_scope = self.environment.child_scope()
            loop_scope.define(statement.variable, elem_type)
            self._check_body_in_scope(statement.body, loop_scope)
            return

        if isinstance(statement, BlockStatement):
            for stmt in statement.statements:
                self.check_statement(stmt)
            return

        if isinstance(statement, (BreakStatement, ContinueStatement)):
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

    def _infer_variable(self, expression: VariableExpression) -> FalxType:
        variable_type = self.environment.resolve(expression.name)

        if variable_type is None:
            raise TypeCheckError(
                f"Variable '{expression.name}' is not defined.",
                expression.location,
            )

        return variable_type

    def _infer_binary(self, expression: BinaryExpression) -> FalxType:
        left_type = self.infer_expression(expression.left)
        right_type = self.infer_expression(expression.right)
        operator = expression.operator.tokenKind
        location = expression.location

        return self._infer_binary_types(left_type, right_type, operator, location)

    def _infer_binary_types(
        self,
        left_type: FalxType,
        right_type: FalxType,
        operator: TokenKind,
        location,
    ) -> FalxType:
        if isinstance(left_type, UnionType):
            return make_union(*(
                self._infer_binary_types(m, right_type, operator, location)
                for m in left_type.members
            ))

        if isinstance(right_type, UnionType):
            return make_union(*(
                self._infer_binary_types(left_type, m, operator, location)
                for m in right_type.members
            ))

        if left_type == ANY or right_type == ANY:
            if operator in {
                TokenKind.EQUAL_EQUAL, TokenKind.BANG_EQUAL,
                TokenKind.GREATER, TokenKind.GREATER_EQUAL,
                TokenKind.LESS, TokenKind.LESS_EQUAL,
            }:
                return BOOL
            return ANY

        numeric_types = (INT, FLOAT)

        if operator == TokenKind.PLUS:
            if left_type == STRING and right_type in {STRING, INT, FLOAT, BOOL}:
                return STRING
            if right_type == STRING and left_type in {STRING, INT, FLOAT, BOOL}:
                return STRING

            if left_type in numeric_types and right_type in numeric_types:
                return self._numeric_result(left_type, right_type)

        elif operator in {TokenKind.MINUS, TokenKind.STAR, TokenKind.SLASH, TokenKind.PERCENT}:
            if left_type in numeric_types and right_type in numeric_types:
                return self._numeric_result(left_type, right_type)

        elif operator in {TokenKind.EQUAL_EQUAL, TokenKind.BANG_EQUAL, TokenKind.GREATER, TokenKind.GREATER_EQUAL, TokenKind.LESS, TokenKind.LESS_EQUAL}:
            return BOOL

        raise TypeCheckError(
            f"Operator '{operator.name}' cannot be applied to '{left_type}' and '{right_type}'.",
            location,
        )

    @staticmethod
    def _numeric_result(left_type: FalxType, right_type: FalxType) -> FalxType:
        if FLOAT in (left_type, right_type):
            return FLOAT

        return INT

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

    def _infer_function_call(self, expression: FunctionCallExpression) -> FalxType:
        if isinstance(expression.callee, VariableExpression):
            builtin_name = expression.callee.name
            if builtin_name in {"output", "assert"}:
                for arg in expression.arguments:
                    self.infer_expression(arg)
                return VOID
            if builtin_name == "typeof":
                for arg in expression.arguments:
                    self.infer_expression(arg)
                return STRING
            if builtin_name in {"require", "request"}:
                for arg in expression.arguments:
                    self.infer_expression(arg)
                return ANY

        callee_type = self.infer_expression(expression.callee)

        for arg in expression.arguments:
            self.infer_expression(arg)

        if isinstance(callee_type, FunctionType):
            return callee_type.return_type

        if callee_type == ANY:
            return ANY

        raise TypeCheckError(
            f"Type '{callee_type}' is not callable.",
            expression.location,
        )


    def _check_function_declaration(self, statement: FunctionDeclaration) -> None:
        location = statement.location

        seen: set[str] = set()
        for param in statement.parameters:
            if param.name in seen:
                raise TypeCheckError(
                    f"Duplicate parameter '{param.name}' "
                    f"in function '{statement.name}'.",
                    location,
                )
            seen.add(param.name)

        param_types: list[FalxType] = []
        for param in statement.parameters:
            annotation = param.type_annotation
            default = param.defaultValue

            if annotation is not None and default is not None:
                declared_type = self.resolver.resolve(annotation)
                inferred_type = self.infer_expression(default)

                if not is_assignable(declared_type, inferred_type):
                    raise TypeCheckError(
                        f"Default value of type '{inferred_type}' is not "
                        f"assignable to parameter '{param.name}' "
                        f"of declared type '{declared_type}'.",
                        location,
                    )

                param_types.append(declared_type)

            elif annotation is not None:
                param_types.append(self.resolver.resolve(annotation))

            elif default is not None:
                param_types.append(self.infer_expression(default))

            else:
                param_types.append(ANY)

        function_scope = self.environment.child_scope()
        for param, ptype in zip(statement.parameters, param_types):
            function_scope.define(param.name, ptype)

        self._check_body_in_scope(statement.body, function_scope)
        return_types = self._collect_return_types(statement.body, function_scope)

        if return_types:
            inferred_return = make_union(*return_types)
        else:
            inferred_return = VOID

        if statement.return_type_annotation is not None:
            declared_return = self.resolver.resolve(
                statement.return_type_annotation,
            )

            if not return_types and declared_return != VOID:
                raise TypeCheckError(
                    f"Function '{statement.name}' declares return type "
                    f"'{declared_return}' but does not return a value.",
                    location,
                )

            if return_types and not is_assignable(declared_return, inferred_return):
                raise TypeCheckError(
                    f"Function '{statement.name}' returns "
                    f"'{inferred_return}' which is not assignable "
                    f"to declared return type '{declared_return}'.",
                    location,
                )

            effective_return = declared_return
        else:
            effective_return = inferred_return

        func_type = FunctionType(
            parameter_types=tuple(param_types),
            return_type=effective_return,
        )

        try:
            self.environment.define(statement.name, func_type)
        except ValueError as error:
            raise TypeCheckError(str(error), location) from error

    def _check_body_in_scope(self, body: list[Statement], scope: TypeEnvironment,) -> None:
        previous = self.environment
        self.environment = scope
        try:
            for stmt in body:
                self.check_statement(stmt)
        finally:
            self.environment = previous

    def _collect_return_types(self, body: list[Statement], scope: TypeEnvironment,) -> list[FalxType]:
        result: list[FalxType] = []

        previous = self.environment
        self.environment = scope
        try:
            for stmt in body:
                if isinstance(stmt, ReturnStatement):
                    if stmt.value is not None:
                        result.append(self.infer_expression(stmt.value))
                    else:
                        result.append(VOID)

                elif isinstance(stmt, IfStatement):
                    result.extend(
                        self._collect_return_types(stmt.body, scope),
                    )
                    if stmt.elseBody is not None:
                        if isinstance(stmt.elseBody, BlockStatement):
                            result.extend(
                                self._collect_return_types(
                                    stmt.elseBody.statements, scope,
                                ),
                            )
                        elif isinstance(stmt.elseBody, IfStatement):
                            result.extend(
                                self._collect_return_types(
                                    [stmt.elseBody], scope,
                                ),
                            )
                        elif isinstance(stmt.elseBody, ReturnStatement):
                            if stmt.elseBody.value is not None:
                                result.append(
                                    self.infer_expression(stmt.elseBody.value),
                                )
                            else:
                                result.append(VOID)

                elif isinstance(stmt, (WhileStatement, ForeachStatement)):
                    result.extend(
                        self._collect_return_types(stmt.body, scope),
                    )

                elif isinstance(stmt, BlockStatement):
                    result.extend(
                        self._collect_return_types(stmt.statements, scope),
                    )

                elif isinstance(stmt, FunctionDeclaration):
                    pass
        finally:
            self.environment = previous

        return result
