# Changelog

All notable changes to Falx will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) once it reaches 1.0.

## [Unreleased]

- REPL mode

## [0.2.0]

### Added

- `assert` builtin for validating conditions at runtime
- Module system using `require` for splitting programs across multiple `.flx` files
- Circular module dependency detection
- Recursion depth protection
- Dedicated exceptions and diagnostic codes for individual lexer, parser, runtime and module errors
- Improved runtime error messages with source locations
- Improved diagnostic help and notes
- Additional runtime type, iteration, assertion, function, property and indexing errors

### Changed

- Runtime errors now provide more specific diagnostic information instead of using Python erros
- Error messages now include the relevant source location where available

## [0.1.0]

Initial implementation, ported from an earlier Java prototype.

### Added

- Lexer, recursive decent parser, tree walking interpreter
- Variables (`let`, `const`) and assignment, including compound assignment (`+=`, `-=`, etc) and `++`/ `--`
- Functions as first class values with closures
- Structs with fields, default field values, and methods that can read and mutate enclosing state
- Arrays, with literals and built in methods
- Maps, with literals and built in methods
- Comments (`//`)
- String literals with escape sequences, indexing and methods
- Numeric literals (integer and float)
- `null` and equality handling
- Built in functions: `output`, `request`, `typeof`
- CLI: `run`, `--test`, `--version`
- Test suite covering successful programs, expected failures, and larger example programs

[Unreleased]: https://github.com/SharkBlocks00/Falx/comparev0.1.0...HEAD
[0.1.0]: https://github.com/SharkBlocks00/Falx/releases/tag/v0.1.0
[0.2.0]: https://github.com/SharkBlocks00/Falx/releases/tag/v0.2.0
