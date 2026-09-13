from pathlib import Path

from src.ast.Statement import Statement
from src.lexer.Lexer import Lexer
from src.parser.Parser import Parser


class ModuleParser:
    def __init__(self):
        self.lexer = None
        self.parser = None


    def parse(self, path: Path) -> list[Statement]:
        source = path.read_text(encoding="utf-8")

        self.lexer = Lexer(source)
        tokens = self.lexer.lex()
        self.parser = Parser(tokens)

        return self.parser.parse()