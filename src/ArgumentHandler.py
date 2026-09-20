import argparse
from pathlib import Path


class ArgumentHandler:
    def __init__(self, arguments: list[str]):
        self.arguments = arguments

    def handleArguments(self) -> argparse.Namespace:
        parser = argparse.ArgumentParser(
            prog="falx",
            description="The Falx programming language"
        )

        parser.add_argument(
            "--version",
            "-v",
            action="version",
            version="Falx v0.2.0"
        )

        subparsers = parser.add_subparsers(dest="command")

        runParser = subparsers.add_parser(
            "run",
            aliases=["--file", "-f"]
        )

        runParser.add_argument(
            "file",
            type=Path
        )

        testParser = subparsers.add_parser(
            "test",
            aliases=["-t"]
        )

        testParser.add_argument(
            "--update-snapshots",
            action="store_true"
        )

        return parser.parse_args(self.arguments[1:])