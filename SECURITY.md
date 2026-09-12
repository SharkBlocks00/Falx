# Security Policy

## Supported Versions

Falx is pre-1.0 and does not yet maintain long-term support branches. Security fixes, if needed, will be applied to `master` only.

| Version | Supported |
| ------- | --------- |
| 0.x     | [X]       |

## Reporting a Vulnerability

Falx is a tree-walking interpreter that executes untrusted `.flx` source directly in the host Python process. If you find a way to escape the interpreter's intended sandboxing (e.g. accessing the host filesystem, network, or Python internals from within a Falx script in ways not explicitly documented as built-ins), please report it responsibly rather than opening a public issue:

1. Open a [GitHub Security Advisory](https://github.com/SharkBlocks00/Falx/security/advisories/new) for this repository, **or**
2. Contact the maintainer directly via their GitHub profile.

Please include:

- A description of the issue and its potential impact
- Steps to reproduce, ideally as a minimal `.flx` script
- Any suggested fix, if you have one

You should expect an initial response within a few days. Given the project's current size, there is no bug bounty program.

## Disclosure

Please give the maintainer a reasonable amount of time to address the issue before any public disclosure.
