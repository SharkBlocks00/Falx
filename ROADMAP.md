# Falx Roadmap

This is a rough picture of where Falx is heading. There are no promises regarding new features, as Falx is maintained in spare time, and priorities may shift. Items move between sections as they are picked up; see [Issues](https://github.com/SharkBlocks00/Falx/issues) and [`CHANGELOG.md`](CHANGELOG.md) for what will and has been implemented respectively.

Have an opinion on the ordering, or want to pick something up? Open an issue or see [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Now

Features currently in development or planned for the immediate future:

* [ ] **Expanded standard library** - math functions, random methods, filesystem utilities and networking modules
* [ ] **Improved module system** - continue improving module discovery, relative imports and module organization
* [ ] **CLI improvements** - improve commands, help output, version information and developer experience

## Planned

Larger features that fit Falx's current direction:

* [ ] **Packaging** - make Falx installable through `pip` or available as a standalone executable rather than requiring `python -m src.Main`
* [ ] **Ternary operators** - add a concise conditional expression syntax
* [ ] **Pattern matching** - potentially introduce a `match`/`switch`-style statement
* [ ] **OOP** - classes, class inheritance, and upgraded structs with additional features
* [ ] **File and process utilities** - provide a cleaner standard interface for interacting with the filesystem and processes
* [ ] **Improved string functionality** - expand the built-in string API with additional manipulation and inspection methods

## Under consideration

Bigger, less certain ideas:

* [ ] **Static/optional type annotations**
* [ ] **Attributes/decorators** - annotations such as `[decorator]` for functions and classes
* [ ] **Bytecode compiler and VM** - a compiled bytecode execution path as a faster alternative to the current tree-walking interpreter
* [ ] **Editor tooling** - syntax highlighting and an LSP
* [ ] **Language specification and documentation site** - a formal specification and dedicated documentation once the language syntax and semantics are stable

## Later

These are not expected to be implemented soon, but are worth keeping on the roadmap:

* [ ] **Multithreading / concurrency primitives**
* [ ] **Package manager and third-party dependency ecosystem**
* [ ] **Backwards compatibility guarantees** - pre-1.0 breaking changes are expected

## Recently shipped

### v0.3.0

* [x] **Default function arguments**
* [x] **REPL mode**
* [x] **Tuples**
* [x] **Tuple destructuring**
* [x] **Improved module system**
* [x] **Module caching and canonical module resolution**
* [x] **Relative and nested module resolution**
* [x] **Improved `falx test` command**
* [x] **Test filtering**
* [x] **Snapshot testing**
* [x] **Expected diagnostic-code testing**
* [x] **Test timeouts and multiprocessing**
* [x] **Recursive test discovery**
* [x] **Improved runtime diagnostics**
* [x] **Improved lexical scoping**
* [x] **Improved `break` and `continue` semantics**
* [x] **Improved parser and lexer diagnostics**
* [x] **Expanded runtime, parser, lexer and integration test coverage**
* [x] **Numerous runtime and interpreter bug fixes**

### Previous releases

* [x] **Line/column-aware runtime error messages**
* [x] **Friendly parser errors**
* [x] **Unique, separate errors instead of generic Python errors**
* [x] **Module system**

See [`CHANGELOG.md`](CHANGELOG.md) for the complete history.
