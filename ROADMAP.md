# Falx Roadmap

This is a rough picture of where Falx is heading. There are no promises with regards to releasing new features, as Falx is maintained
in spare time, and as such, priorities will shift. Items move between sections as they're picked up; see [issues](https://github.com/SharkBlocks00/Falx/issues)
and [`CHANGELOG.md`](CHANGELOG.md) for what will and has been implemented respectively.

Have an opinion on ordering or want to pick something up? Open an issue - see [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Now

Features currently in development:

- [ ] **Default function arguments** - allow functions to define default values for parameters
- [ ] **REPL mode** - running `falx` with no arguments or `falx -c` should go into an interactive REPL
- [ ] **Expanded stdlib** - math functions, random methods, networking modules

## Planned

Larger features that fit the language's current direction:

- [ ] **Packaging** - make Falx either pip installable or a standalone executable rather than `python -m src.Main`
- [ ] **Ternary operators** and possibly a `match`/`switch` style statement
- [ ] **OOP** - classes, class inheritance, essentially upgraded structs with more features

## Under consideration

Bigger, less certain ideas:

- [ ] Static/optional type annotations
- [ ] Attributes/decorators, annotations such as `[decorator]` for functions and classes
- [ ] A bytecode compiler and VM, faster alternative to the current tree walking interpreter
- [ ] Editor tooling (syntax highlighting, an LSP)
- [ ] A proper language specification / docs site, once syntax and ideas are stable

## Later 

These will not be implemented any time soon, but worth putting on for consideration:

- [ ] Multi threading / concurrency primitives
- [ ] A package manager or third party dependency ecosystem
- [ ] Backwards compatibility guarantees - pre-1.0, breaking changes are expected

## Recently shipped

- [x] Line/column aware runtime error messages
- [x] Friendly parser errors
- [x] Unique, separate errors instead of generic Python errors
- [x] Module system

See [`CHANGELOG.md`](CHANGELOG.md) for the full history. 
