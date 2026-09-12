# Contributing to Falx

Thank you for your interest in contributing. Falx is in a very early stage, so the contributing process is very lightweight - expect
it to grow more formal as the project matures later on.

## Before you start

For anything beyond a small fix (typos, obvious bugs), please open an issue first to discuss the change. This avoids wasted effort on
pull requests that don't fit Falx's direction, especially whilst the language design is still moving. Check [`ROADMAP.md`](ROADMAP.md) first
as it may already be tracked there or flagged as something not to be worked on until much later.

## Development setup

Falx has no external dependencies at runtime.

```bash
git clone https://github.com/SharkBlocks00/Falx.git
cd Falx
```

For linting, install [ruff](https://docs.astral.sh/ruff/):

```bash
pip install ruff
```

## Making changes

1. Fork the repo and create a branch from `master`.
2. Make your changes.
3. Add or update tests in `tests/success`, `tests/failure`, or `tests/programs` as appropriate (see below).
4. Run the test suite and linter locally:

```bash
python -m src.Main --test
ruff check .
```

5. Open a pull request describing what changed and why.

## Adding tests

Tests are plain `.flx` scripts, grouped by expected outcome:

- `tests/success/` - scripts that should run without error. One feature or behaviour per file where possible
- `tests/failure/` - scripts that are expected to raise an error.
- `tests/programs/` - larger, more realistic example programs used as integration test.

The test runner (`python -m src.Main --test`) picks these up automatically based on their directory.

## Code style

The interpreter's Python code uses `camelCase` for methods and variables (a holdover from its origins coming from Java) rather than
the more typical `snake_case`. Some files may use `snake_case` instead, so please match the existing style within a file rather than mixing conventions.

## Reporting bugs

Please include:

- The `.flx` script that triggers the issue, or a minimal reproduction
- What you expected to happen
- What actually happened, including any traceback

## Code of Conduct

This project follows a [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you agree to uphold it.
