<!-- SPDX-License-Identifier: 0BSD -->

<!-- markdownlint-capture -->
<!-- markdownlint-disable first-line-h1 -->
[![GitHub license](https://img.shields.io/github/license/EliahKagan/sig)](https://github.com/EliahKagan/sig/blob/main/LICENSE)
[![Run experiment](https://github.com/EliahKagan/sig/actions/workflows/main.yml/badge.svg)](https://github.com/EliahKagan/sig/actions/workflows/main.yml)
[![Open in GitHub Codespaces](https://img.shields.io/badge/GitHub_Codespaces-Click_to_Open-blue)](https://codespaces.new/EliahKagan/sig)
<!-- markdownlint-restore -->

# sig - Unix signals demonstration with subprocesses

This is a demonstration of signals in Unix-like systems, when a process
receiving a signal has its own subprocesses.

A C compiler and `make` are needed.

## License

The code in this repository is licensed under
[0BSD](https://spdx.org/licenses/0BSD.html), a
[“public-domain equivalent”](https://en.wikipedia.org/wiki/Public-domain-equivalent_license)
license.

See [**`LICENSE`**](LICENSE).

## Usage

Run:

```sh
./run-experiment
```

*Or* experiment interactively by building `sig` and running it directly (you
may need to kill the subprocesses yourself).

The `run-experiment` script takes care of building the `sig` program from
`sig.c` if this has not been done. You can also build `sig` by running:

```sh
make
```

Then to run `sig`:

```sh
./sig
```

The `sig` command accepts a signal number as an argument. For example, to make
it use `SIGKILL` instead of its default of `SIGTERM`, run:

```sh
./sig 9
```

Or to have the `run-experiment` script call `sig` that way:

```sh
./run-experiment 9
```
