from unittest import case

from src.ast.SourceLocation import SourceLocation
from src.tokens.Token import Token
from src.tokens.TokenKind import TokenKind


class Lexer:
    def __init__(self, source: str):
        self.source: str = source
        self.current: int = 0
        self.line: int = 1
        self.start: int = 0
        self.column: int = 1
        self.startColumn: int = 0
        self.tokens: list[Token] = []
        self.KEYWORDS: dict[str, TokenKind] = initKeywords()

    def lex(self) -> list[Token]:
        while not self._isAtEnd():
            self.start = self.current
            self._scanToken()

        self.tokens.append(Token(TokenKind.EOF, "", None, SourceLocation("", self.line, self.column)))
        return self.tokens

    def _scanToken(self) -> None:
        c: str = self._advance()

        if c == "\n":
            self.line += 1
            self.column = 1
            return

        self.startColumn = self.column
        match c:
            case "(": self._addToken(TokenKind.LEFT_PAREN)
            case ")": self._addToken(TokenKind.RIGHT_PAREN)
            case "[": self._addToken(TokenKind.LEFT_BRACKET)
            case "]": self._addToken(TokenKind.RIGHT_BRACKET)
            case "{": self._addToken(TokenKind.LEFT_BRACE)
            case "}": self._addToken(TokenKind.RIGHT_BRACE)
            case ",": self._addToken(TokenKind.COMMA)
            case ".": self._addToken(TokenKind.DOT)
            case ":": self._addToken(TokenKind.COLON)
            case ";": self._addToken(TokenKind.SEMICOLON)
            case "%":
                if self._match("="):
                    self._addToken(TokenKind.PERCENT_EQUAL)
                else:
                    self._addToken(TokenKind.PERCENT)
            case "!":
                if self._match("="):
                    self._addToken(TokenKind.BANG_EQUAL)
                else:
                    self._addToken(TokenKind.BANG)
            case "+":
                if self._match("="):
                    self._addToken(TokenKind.PLUS_EQUAL)
                elif self._match("+"):
                    self._addToken(TokenKind.PLUS_PLUS)
                else:
                    self._addToken(TokenKind.PLUS)
            case "-":
                if self._match("-"):
                    self._addToken(TokenKind.MINUS_MINUS)
                elif self._match("="):
                    self._addToken(TokenKind.MINUS_EQUAL)
                else:
                    self._addToken(TokenKind.MINUS)
            case "*":
                if self._match("*"):
                    self._addToken(TokenKind.STAR_STAR)
                elif self._match("="):
                    self._addToken(TokenKind.STAR_EQUAL)
                else:
                    self._addToken(TokenKind.STAR)
            case "/":
                if self._match("/"):
                    self._addToken(TokenKind.SLASH_SLASH)
                elif self._match("="):
                    self._addToken(TokenKind.SLASH_EQUAL)
                else:
                    self._addToken(TokenKind.SLASH)
            case "=":
                self._addToken(TokenKind.EQUAL_EQUAL if self._match("=") else TokenKind.EQUAL)
            case '"': self._string()
            case ">":
                self._addToken(TokenKind.GREATER_EQUAL if self._match("=") else TokenKind.GREATER)
            case "<":
                self._addToken(TokenKind.LESS_EQUAL if self._match("=") else TokenKind.LESS)
            case "|":
                if self._match("|"):
                    self._addToken(TokenKind.OR)
                else:
                    raise RuntimeError("Expected '||', found '|'.")
            case "&":
                if self._match("&"):
                    self._addToken(TokenKind.AND)
                else:
                    raise RuntimeError("Expected '&&', found '&'.")
            case _:
                if c.isdigit(): self._number()
                elif c.isalpha(): self._identifier()



    def _string(self) -> None:
        value: str = ""
        self.startColumn = self.column

        while not self._isAtEnd():
            c: str = self._peek()

            if c == '"':
                self._advance()
                self._addToken(TokenKind.STRING, value)
                return
            if c == "\\":
                self._advance()
                escaped: str = self._advance()
                match escaped:
                    case "n": value += "\n"
                    case "t": value += "\t"
                    case "r": value += "\r"
                    case "f": value += "\n"
                    case "b": value += "\n"
                    case '"': value += '"'
                    case "\\": value += '\\'
                    case _: value += c
            else:
                value += self._advance()

    def _number(self) -> None:
        start: int = self.start
        self.startColumn = self.column

        while not self._isAtEnd() and self._peek().isdigit():
            self._advance()

        if self._peek() == "." and self._peekNext().isdigit():
            self._advance()
            while not self._isAtEnd() and self._peek().isdigit():
                self._advance()

        text: str = self.source[start:self.current]
        self._addToken(TokenKind.NUMBER, float(text))


    def _identifier(self) -> None:
        start: int = self.start
        self.startColumn = self.column
        while not self._isAtEnd() and (self._peek().isalnum() or self._peek() == "_"):
            self._advance()

        value: str = self.source[start:self.current]

        if value == "true" or value == "false":
            self._addToken(TokenKind.BOOLEAN, bool(value))
            return

        self._addToken(self.KEYWORDS[value] if self.KEYWORDS.get(value) else TokenKind.IDENTIFIER, value)


    def _match(self, c: str) -> bool:
        """Checks if the passed char matches the current char,
        and if so, advances the token stream onwards
        """
        if self._isAtEnd(): return False
        if self._peek() != c: return False
        self._advance()
        return True

    def _addToken(self, kind: TokenKind, literal: object = None) -> None:
        lexeme: str = self.source[self.start:self.current]
        self.tokens.append(Token(
            kind, lexeme, literal,
            SourceLocation("", self.line, self.startColumn)
        ))

    def _peek(self) -> str:
        if self._isAtEnd():
            return "\0"
        return self.source[self.current]

    def _peekNext(self) -> str:
        if self._isAtEnd():
            return "\0"
        return self.source[self.current + 1]

    def _advance(self) -> str:
        c: str = self.source[self.current]
        self.current += 1
        self.column += 1
        return c

    def _isAtEnd(self) -> bool:
        return self.current >= len(self.source)



def initKeywords() -> dict[str, TokenKind]:
    return {
        "let": TokenKind.LET,
        "const": TokenKind.CONST,
        "if": TokenKind.IF,
        "else": TokenKind.ELSE,
        "elseif": TokenKind.ELSEIF,
        "while": TokenKind.WHILE,
        "foreach": TokenKind.FOREACH,
        "return": TokenKind.RETURN,
        "break": TokenKind.BREAK,
        "continue": TokenKind.CONTINUE,
        "define": TokenKind.DEFINE,
        "func": TokenKind.FUNC,
        "struct": TokenKind.STRUCT,
    }