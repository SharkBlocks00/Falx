# Changelog

All notable changes to Falx will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) once it reaches 1.0.

## [Unreleased]

Nothing here yet!

## [0.3.0]

### Added

* Added an interactive REPL when Falx is run without a command.
* Added `exit()` support for leaving the REPL.
* Added default values for function parameters.
* Added validation preventing required parameters from appearing after parameters with defaults.
* Added tuples as a new immutable, indexable and iterable value type.
* Added tuple destructuring for variable declarations and assignments.
* Added tuple-specific validation and diagnostics for invalid destructuring patterns and mismatched value counts.
* Added `--filter` to `falx test` for running a subset of the test suite.
* Added `--update-snapshots` to `falx test` for regenerating `.out` output snapshots.
* Added stdout/stderr snapshot testing for successful programs.
* Added expected diagnostic-code annotations (`// Expects: <error code>`) to failure tests.
* Added per-test timeouts so a hanging test cannot block the entire test suite.
* Added multiprocessing-based test execution and worker recovery after a test timeout.
* Added clearer failure output showing expected and actual exception types when a failure test throws the wrong exception.
* Added automatic recursive discovery of `.flx` tests under the test directories.
* Added additional runtime diagnostics for invalid values, indexing, hashing, inversion, immutable values, destructuring counts and duplicate functions.
* Added canonical module path resolution and module caching to the existing `require()` module system.
* Added relative module resolution so a module can resolve another module relative to its own location.

### Changed

* `require()` now resolves modules canonically, preventing the same module from being loaded multiple times through different paths.
* Modules are cached after loading and reused when required again.
* Module `require()` functions now retain the path they are relative to, allowing nested and relative imports to resolve correctly.
* `falx test` now reports and validates expected exception codes instead of only checking whether a test failed.
* Test output is now captured and compared against `.out` snapshots for successful tests.
* The test runner now reports per-test execution time and total suite time.
* The CLI argument parser was migrated to `argparse` and now supports structured commands and aliases for running and testing files.
* `falx test` can now be invoked with `-t`; file execution supports `-f` / `--file` aliases.
* `break` and `continue` now use loop-specific exceptions rather than function-context exceptions.
* `break` and `continue` now behave correctly inside `foreach` loops and functions containing loops.
* Logical expressions now evaluate in the correct environment.
* Variable declarations inside `if` statements now use the correct lexical environment.
* Struct property access and assignment now use the correct environment rather than the global environment.
* Struct constructors now correctly reject extra arguments and can access variables from their enclosing environment.
* `typeof()` now reports Falx type names instead of internal Python class names.
* Boolean values now use Falx boolean representations when printed.
* Boolean comparison is no longer accepted as a normal value comparison.
* Missing map properties now produce Falx `null` instead of Python `None`.
* Maps are no longer hashable.
* Multi-dimensional arrays now retain their nested structure when formatted.
* `String.split()` now returns all elements produced by the split operation.
* Division handling was corrected to avoid precision/identity issues.
* Negative indexing is accepted correctly where supported.
* Built-in/runtime operations now convert more Python-level errors into Falx runtime exceptions.
* Runtime indexing now provides more specific diagnostics for arrays, strings and maps.
* Lexer and parser diagnostics now include missing filenames and more accurate source locations.
* Escape handling for `\f` and `\b` was corrected.

### Fixed

* Fixed null checks not working correctly across Falx values.
* Fixed `FalxMap.clear()` leaving the map's `size` property incorrect.
* Fixed `break` inside a function incorrectly breaking an enclosing loop.
* Fixed `break` and `continue` handling in `foreach` loops.
* Fixed the parser hanging indefinitely on unterminated struct bodies.
* Fixed EOF inside a string being reported as a generic expression error instead of an unterminated-string error.
* Fixed raw Python tracebacks being exposed for unhandled lexer and parser errors.
* Fixed lexer methods overwriting the original token start column while reporting diagnostics.
* Fixed exceptions overwriting an already-correct `SourceLocation`.
* Fixed `typeof()` returning internal Python names for some Falx values.
* Fixed booleans printing as Python `True`/`False` values.
* Fixed missing map properties returning Python `None` instead of Falx `null`.
* Fixed `\f` and `\b` escape sequences being interpreted as newlines.
* Fixed incorrect lexical scoping for `if` statements, structs, properties and nested environments.
* Fixed struct constructors accepting too many arguments.
* Fixed several built-in operations leaking Python exceptions instead of Falx exceptions.
* Fixed division precision and related numeric-division edge cases.
* Fixed recursion depth being tracked without actually enforcing the configured limit.
* Fixed negative indexes being rejected incorrectly and invalid-index calculations producing incorrect negative sizes.
* Fixed incorrect indexing access and incorrect exceptions for several index operations.
* Fixed the test runner starting its timer before the worker process was ready.
* Fixed `falx --test` entering the REPL after completing the tests.

### Tests

* Added success and failure tests for default function arguments.
* Added success and failure tests for tuples and destructuring.
* Added tests for `break`/`continue` control-flow edge cases.
* Added tests for parser and lexer error conditions.
* Added tests for source-location and diagnostic behavior.
* Added tests for null handling, map behavior, boolean behavior, `typeof()`, multidimensional arrays and string splitting.
* Added tests for scoping and struct-environment behavior.
* Added tests for nested and repeated module imports.
* Added tests for module caching and relative module resolution.
* Added tests covering integer division, remainder, mixed numeric division, large integer division and division identity.
* Expanded the failure suite to validate expected diagnostic codes.
* Added output snapshots for successful programs and integration programs.
* Added additional runtime and implementation-level test coverage.

### Repository / Tooling

* Updated CI and pull-request/test configuration for the expanded CLI and test runner.
* Removed tracked Python cache files from the repository.
* Updated documentation and roadmap entries for the new features.


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

- Runtime errors now provide more specific diagnostic information instead of using Python errors
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
[0.3.0]: https://github.com/SharkBlocks00/Falx/releases/tag/v0.3.0
