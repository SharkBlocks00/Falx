# Falx

A small, simple tree walking scripting language implemented in Python.

[![CI](https://github.com/SharkBlocks00/Falx/actions/workflows/ci.yml/badge.svg)](https://github.com/SharkBlocks00/Falx/actions/workflows/ci.yml)
[![License: Apache 2.0](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-0.1.0-orange.svg)](CHANGELOD.md)
[![Python](https://img.shields.io/badge/python-3.11%2B-blue.svg)](https://www.python.org)
[![Status](https://img.shields.io/badge/status-experimental-red.svg)](#project-status)

Falx is a dynamically typed, C like scripting language with structs, first class functions, and closures.

## Features

- Variables with `let` and `const`
- First class functions and closures
- Structs with fields, default values, and methods
- Arrays with literals and built in methods
- Maps with literals and built in methods
- Control flow: `if` / `elseif` / `else`, `while`, `foreach`
- `break`, `continue`, `return`
- Native string and number types with built in methods
- A handful of built in functions: `output`, `request`, `typeof`

See [`tests/programs`](tests/programs) for large example scripts.

## Getting started

Falx requires Python 3.11+ and has no external dependancies.

```bash
git clone https://github.com/SharkBlocks00/Falx.git
cd Falx
```

Run a script:

```bash
python -m src.Main run path/to/script.flx
```

Check the version:

```bash
python -m src.Main --version
```

Run the test suite:

```bash
python -m src.Main --test
```

## Project status

Falx is a few days old and under active, early development. The language, its syntax and its standard library 
should all be considered unstable and subject to breaking changes without any notice. It is not yet 
recommended for anything beyond experimentation.

## Contributing

Contributions, bug reports and ideas are very much welcome. Please read [`CONTRIBUTING.md`](CONTRIBUTING.MD) 
before opening a pull request, and note this project follows a [Code of Conduct](CODE_OF_CONDUCT.md).

## Security

To report a security issue, please see [`SECURITY.md`](SECURITY.md).

## License

Falx is licensed under the [Apache License 2.0](LICENSE).
