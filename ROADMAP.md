# Falx Roadmap

This is a rough picture of where Falx is heading. There are no promises with regards to releasing new features, as Falx is maintained
in spare time, and as such, priorities will shift. Item move between sections as they're picked up; see [issues](https://github.com/SharkBlocks00/Falx/issues)
and [`CHANGELOG.md`](CHANGELOG.md) for what will and has been implemented respecitvely.

Have an opinion on ordering or want to pick something up? Open an issue - see [`CONTRIBUTING.md`](CONTRIBUTING.md).

## Now

Small items that will be coming soon:

- [ ] Line/column aware runtime error messages
- [ ] Friendly parser errors (`Expected ';' but found 'X' on line N`)
- [ ] Unique, separate errors, instead of just `RuntimeError`s and `Exceptions`

## Planned

Larger features that fit the language's current direction:

- [ ] **REPL mode** - running `falx` with no arguments should drop into an interactive REPL
- [ ] **Module system** - some form of `require` to split a program across multiple `.flx` files
- [ ] **Expanded stdlib** - math functions, random methods, more numeric methods
- [ ] **Packaging** - make Falx either pip installable or a standalone executable rather than `python -m src.Main`
- [ ] **Struct inheritance or composition** - some way to share behavior between structs
- [ ] **Ternary operators** and possibly a `match`/`switch` style statement
- [ ] **OOP** - classes, class inheritance, essentially upgraded structs with more features

## Under consideration

Bigger, less certain ideas:

- [ ] Static/optional type annotations
- [ ] A bytecode compiler and VM, faster alternative to the current tree walking interpreter
- [ ] Editor tooling (syntax highlighting, a lsp)
- [ ] A proper language specification / docs site, once syntax and ideas are stable

## Later roadmap

These will not be implemented any time soon, but worth putting on for consideration:

- [ ] Multi threading / concurrency primatives
- [ ] A package manager or third party dependance ecosystem
- [ ] Backwards compatibility guarantees - pre-1.0, breaking changes are expected

## Recently shipped

See [`CHANGELOG.md`](CHANGELOG.md) for the full history. 
