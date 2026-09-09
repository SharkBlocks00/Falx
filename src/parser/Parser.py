from src.ast.Expression import Expression
from src.ast.SourceLocation import SourceLocation
from src.ast.Statement import Statement
from src.ast.expressions.AssignmentExpression import AssignmentExpression
from src.ast.expressions.BinaryExpression import BinaryExpression
from src.ast.expressions.FunctionCallExpression import FunctionCallExpression
from src.ast.expressions.GetExpression import GetExpression
from src.ast.expressions.IndexExpression import IndexExpression
from src.ast.expressions.LogicalExpression import LogicalExpression
from src.ast.expressions.SetExpression import SetExpression
from src.ast.expressions.UnaryExpression import UnaryExpression
from src.ast.expressions.VariableExpression import VariableExpression
from src.ast.expressions.literals.ArrayLiteral import ArrayLiteral
from src.ast.expressions.literals.BooleanLiteral import BooleanLiteral
from src.ast.expressions.literals.MapLiteral import MapLiteral
from src.ast.expressions.literals.NumberLiteral import NumberLiteral
from src.ast.expressions.literals.StringLiteral import StringLiteral
from src.ast.statements.BlockStatement import BlockStatement
from src.ast.statements.BreakStatement import BreakStatement
from src.ast.statements.ContinueStatement import ContinueStatement
from src.ast.statements.ExpressionStatement import ExpressionStatement
from src.ast.statements.ForeachStatement import ForeachStatement
from src.ast.statements.FunctionDeclaration import FunctionDeclaration
from src.ast.statements.IfStatement import IfStatement
from src.ast.statements.ReturnStatement import ReturnStatement
from src.ast.statements.StructDeclaration import StructDeclaration
from src.ast.statements.VariableDeclaration import VariableDeclaration
from src.ast.statements.WhileStatement import WhileStatement
from src.runtime.values.FieldDefinition import FieldDefinition
from src.tokens.Token import Token
from src.tokens.TokenKind import TokenKind


class Parser:
    def __init__(self, tokens: list[Token]):
        self.tokens: list[Token] = tokens
        self.current: int = 0

    def parse(self) -> list[Statement]:
        statements: list[Statement] = []

        while not self._isAtEnd():
            statements.append(self._declaration())
        return statements

    def _declaration(self) -> Statement:
        if self._match(TokenKind.LET): return self._variableDeclaration(True)
        if self._match(TokenKind.CONST): return self._variableDeclaration(False)
        if self._match(TokenKind.FUNC): return self._functionDeclaration()
        if self._match(TokenKind.STRUCT): return self._structDeclaration()

        return self._statement()

    def _statement(self) -> Statement:
        if self._match(TokenKind.IF): return self._ifStatement()
        if self._match(TokenKind.WHILE): return self._whileStatement()
        if self._match(TokenKind.FOREACH): return self._foreachStatement()
        if self._match(TokenKind.BREAK): return self._breakStatement()
        if self._match(TokenKind.CONTINUE): return self._continueStatement()
        if self._match(TokenKind.RETURN): return self._returnStatement()
        if self._match(TokenKind.LEFT_BRACE): return self._blockStatement()

        return self._expressionStatement()

    def _expressionStatement(self) -> Statement:
        expression: Expression = self._expression()

        self._consume(TokenKind.SEMICOLON, "Expected ';' after expression")

        return ExpressionStatement(expression._location, expression)

    def _variableDeclaration(self, mutable: bool) -> VariableDeclaration:
        name: Token = self._consumeIdentifier()

        self._consume(TokenKind.EQUAL, "Expected '=' after variable name")
        initializer: Expression = self._expression()
        self._consume(TokenKind.SEMICOLON, "Expected ';' after variable declaration")

        return VariableDeclaration(name.location, name.lexeme, initializer, mutable)

    def _functionDeclaration(self) -> FunctionDeclaration:
        name: Token = self._consumeIdentifier()

        self._consume(TokenKind.EQUAL, "Expected '=' after function name")
        self._consume(TokenKind.DEFINE, "Expected 'define' after function")
        self._consume(TokenKind.LEFT_PAREN, "Expected '(' after function 'define'")

        parameters: list[str] = []

        if not self._check(TokenKind.RIGHT_PAREN):
            while self._match(TokenKind.COMMA):
                parameter: Token = self._consume(TokenKind.IDENTIFIER, "Expected parameter name")
                parameters.append(parameter.lexeme)

        self._consume(TokenKind.RIGHT_PAREN, "Expected ')' after function parameters")
        self._consumeLeftBrace()
        body: list[Statement] = self._block()

        return FunctionDeclaration(name.location, name.lexeme, parameters, body)

    def _structDeclaration(self) -> StructDeclaration:
        name: Token = self._consume(TokenKind.IDENTIFIER, "Expected struct name after 'struct'")
        self._consumeLeftBrace()
        fields: list[FieldDefinition] = []

        while not self._check(TokenKind.RIGHT_BRACE):
            field: Token = self._consume(TokenKind.IDENTIFIER, "Expected field name")

            defaultValue: Expression = None

            if self._match(TokenKind.EQUAL):
                defaultValue = self._expression()

            self._consume(TokenKind.SEMICOLON, "Expected ';' after struct field")

            fields.append(FieldDefinition(field.lexeme, defaultValue))

        self._consumeRightBrace()
        return StructDeclaration(name.location, name.lexeme, fields)

    def _ifStatement(self) -> IfStatement:
        location: SourceLocation = self._previous().location

        self._consume(TokenKind.LEFT_PAREN, "Expected '(' after 'if'")
        condition: Expression = self._expression()
        self._consume(TokenKind.RIGHT_PAREN, "Expected ')' after if statement condition")

        self._consumeLeftBrace()
        body: list[Statement] = self._block()

        elseBranch: Statement = None

        if self._match(TokenKind.ELSEIF):
            elseBranch = self._ifStatement()
        elif self._match(TokenKind.ELSE):
            self._consumeLeftBrace()
            elseBranch = BlockStatement(location, self._block())

        return IfStatement(location, condition, body, elseBranch)

    def _whileStatement(self) -> WhileStatement:
        location: SourceLocation = self._previous().location
        self._consume(TokenKind.LEFT_PAREN, "Expected '(' after 'while'")

        condition: Expression = self._expression()

        self._consume(TokenKind.RIGHT_PAREN, "Expected ')' after while statement condition")
        self._consumeLeftBrace()

        body: list[Statement] = self._block()

        return WhileStatement(location, condition, body)

    def _block(self) -> list[Statement]:
        statements: list[Statement] = []

        while not self._check(TokenKind.RIGHT_BRACE) and not self._isAtEnd():
            statements.append(self._declaration())

        self._consume(TokenKind.RIGHT_BRACE, "Expected '}'")

        return statements

    def _foreachStatement(self) -> ForeachStatement:
        location: SourceLocation = self._previous().location

        self._consume(TokenKind.LEFT_PAREN, "Expected '(' after foreach statement")
        variable: str = self._consumeIdentifier().lexeme
        self._consume(TokenKind.COLON, "Expected ':' after foreach variable")

        iterable: Expression = self._expression()
        self._consume(TokenKind.RIGHT_PAREN, "Expected ')' after foreach iterable")
        self._consumeLeftBrace()
        body: list[Statement] = self._block()

        return ForeachStatement(location, variable, iterable, body)

    def _returnStatement(self) -> ReturnStatement:
        location: SourceLocation = self._previous().location
        if self._match(TokenKind.SEMICOLON):
            return ReturnStatement(location, None)
        value: Expression = self._expression()
        self._consumeSemicolon()
        return ReturnStatement(location, value)

    def _breakStatement(self) -> BreakStatement:
        location: SourceLocation = self._previous().location
        self._consumeSemicolon()
        return BreakStatement(location)

    def _continueStatement(self) -> ContinueStatement:
        location: SourceLocation = self._previous().location
        self._consumeSemicolon()
        return ContinueStatement(location)

    def _blockStatement(self) -> BlockStatement:
        location: SourceLocation = self._previous().location
        statements: list[Statement] = self._block()
        return BlockStatement(location, statements)

    def _arrayLiteral(self) -> Expression:
        location: SourceLocation = self._previous().location

        elements: list[Expression] = []

        if not self._check(TokenKind.RIGHT_BRACKET):
            while self._match(TokenKind.COMMA):
                elements.append(self._expression())

        self._consume(TokenKind.RIGHT_BRACKET, "Expected ']' after array")
        return ArrayLiteral(location, elements)

    def _mapLiteral(self) -> Expression:
        location: SourceLocation = self._previous().location

        elements: dict[str, Expression] = {}
        iterations: int = 0

        if not self._check(TokenKind.RIGHT_BRACE):
            while self._match(TokenKind.COMMA):
                key: str = self._consumeIdentifier().lexeme
                self._consume(TokenKind.COLON, "Expected ':' to seperate map key and values")
                value: Expression = self._expression()
                elements[key] = value
                iterations += 1

        self._consumeRightBrace()
        return MapLiteral(location, elements)

    def _expression(self) -> Expression:
        return self._assignment()

    def _assignment(self) -> Expression:
        expr: Expression = self._logicalOr()

        if self._match(TokenKind.EQUAL):
            equals: Token = self._previous()
            value: Expression = self._assignment()

            if isinstance(expr, VariableExpression):
                return AssignmentExpression(equals.location, expr.name, value)
            if isinstance(expr, GetExpression):
                return SetExpression(equals.location, expr.obj, expr._property, value)
            if isinstance(expr, IndexExpression):
                return SetExpression(equals.location, expr.obj, expr.index.__str__(), value)
            raise Exception("Invalid assignment target")
        return expr

    def _logicalOr(self) -> Expression:
        expression: Expression = self._logicalAnd()

        while self._match(TokenKind.OR):
            operator: Token = self._previous()

            right: Expression = self._logicalAnd()

            expression = LogicalExpression(operator.location, expression, operator, right)

        return expression

    def _logicalAnd(self) -> Expression:
        expression: Expression = self._equality()

        while self._match(TokenKind.AND):
            operator: Token = self._previous()

            right: Expression = self._equality()

            expression = LogicalExpression(operator.location, expression, operator, right)

        return expression

    def _equality(self) -> Expression:
        expression: Expression = self._comparison()

        while self._match(TokenKind.EQUAL_EQUAL, TokenKind.BANG_EQUAL):
            operator: Token = self._previous()

            right: Expression = self._comparison()

            expression = BinaryExpression(operator.location, expression, operator, right)

        return expression

    def _comparison(self) -> Expression:
        expression: Expression = self._addition()

        while self._match(TokenKind.LESS, TokenKind.LESS_EQUAL, TokenKind.GREATER, TokenKind.GREATER_EQUAL):
            operator: Token = self._previous()

            right: Expression = self._addition()

            expression = BinaryExpression(operator.location, expression, operator, right)

        return expression

    def _addition(self) -> Expression:
        expression: Expression = self._multiplication()

        while self._match(TokenKind.PLUS, TokenKind.MINUS):
            operator: Token = self._previous()

            right: Expression = self._multiplication()

            expression = BinaryExpression(operator.location, expression, operator, right)

        return expression

    def _multiplication(self) -> Expression:
        expression: Expression = self._unary()

        while self._match(TokenKind.STAR, TokenKind.SLASH, TokenKind.PERCENT):
            operator: Token = self._previous()
            right: Expression = self._unary()
            expression = BinaryExpression(operator.location, expression, operator, right)

        return expression


    def _unary(self) -> Expression:
        if self._match(TokenKind.BANG, TokenKind.MINUS):
            operator: Token = self._previous()

            right: Expression = self._unary()

            return UnaryExpression(operator.location, operator, right)
        return self._call()

    def _call(self) -> Expression:
        expression: Expression = self._primary()

        while True:
            if self._match(TokenKind.LEFT_PAREN):
                expression = self._parseCallArguments(expression)
            elif self._match(TokenKind.DOT):
                name: Token = self._consume(TokenKind.IDENTIFIER, "Expected property name")
                expression: GetExpression = GetExpression(name.location, expression, name.lexeme)
            elif self._match(TokenKind.LEFT_BRACKET):
                index: Expression = self._expression()
                self._consume(TokenKind.RIGHT_BRACKET, "Expected ']'")
                expression: IndexExpression = IndexExpression(expression._location, expression, index)
            else: break

        return expression

    def _parseCallArguments(self, callee: Expression) -> Expression:
        arguments: list[Expression] = []

        if not self._check(TokenKind.RIGHT_PAREN):
            arguments.append(self._expression())

            while self._match(TokenKind.COMMA):
                arguments.append(self._expression())
        closingParen: Token = self._consume(TokenKind.RIGHT_PAREN, "Expected ')' after arguments")

        return FunctionCallExpression(closingParen.location, callee, arguments)

    def _primary(self) -> Expression:
        if self._match(TokenKind.NUMBER):
            return NumberLiteral(self._previous())
        if self._match(TokenKind.STRING):
            return StringLiteral(self._previous())
        if self._match(TokenKind.IDENTIFIER):
            identifier: Token = self._previous()
            return VariableExpression(identifier.location, identifier.lexeme)
        if self._match(TokenKind.BOOLEAN):
            return BooleanLiteral(self._previous())
        if self._match(TokenKind.LEFT_PAREN):
            expression: Expression = self._expression()
            self._consume(TokenKind.RIGHT_PAREN, "Expected ')' after an expression")
            return expression
        if self._match(TokenKind.LEFT_BRACKET):
            return self._arrayLiteral()
        if self._match(TokenKind.LEFT_BRACE):
            return self._mapLiteral()
        if self._match(TokenKind.EOF):
            raise Exception("Expected expression, found 'EOF'")
        raise NotImplementedError(f"Other literals not implemented yet: {self._peek().tokenKind}")

    def _peek(self) -> Token:
        return self.tokens[self.current]

    def _peekNext(self) -> Token:
        if self.current + 1 >= len(self.tokens):
            return self.tokens[-1]
        return self.tokens[self.current + 1]

    def _previous(self) -> Token:
        return self.tokens[self.current - 1]

    def _check(self, kind: TokenKind) -> bool:
        return self._peek().tokenKind == kind

    def _checkNext(self, kind: TokenKind) -> bool:
        if self.current + 1 >= len(self.tokens):
            return False
        return self.tokens[self.current +1].tokenKind == kind

    def _match(self, *args: TokenKind) -> bool:
        for kind in args:
            if self._check(kind):
                self._advance()
                return True
        return False


    def _consume(self, kind: TokenKind, errorMsg: str) -> Token:
        if self._check(kind):
            return self._advance()
        raise Exception(errorMsg)


    def _advance(self) -> Token:
        token: Token = self.tokens[self.current]
        self.current += 1
        return token


    def _isAtEnd(self) -> bool:
        return self._peek().tokenKind == TokenKind.EOF

    def _parseStatementsUntil(self, end: TokenKind) -> list[Statement]:
        statements: list[Statement] = []

        while not self._isAtEnd() and not self._check(end):
            statements.append(self._declaration())

        self._consume(end, f"Expected '{end}'")
        return statements

    def _consumeIdentifier(self) -> Token:
        return self._consume(TokenKind.IDENTIFIER, "Expected variable name.")

    def _consumeSemicolon(self) -> Token:
        return self._consume(TokenKind.SEMICOLON, "Expected ';")

    def _consumeLeftBrace(self) -> Token:
        return self._consume(TokenKind.LEFT_BRACE, "Expected '{'")

    def _consumeRightBrace(self) -> Token:
        return self._consume(TokenKind.RIGHT_BRACE, "Expected '}'")