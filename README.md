<!-- SPDX-License-Identifier: 0BSD -->

# sig - Unix signals demonstration

This is a demonstration of signals in Unix-like systems, when a process
receiving a signal has its own subprocesses.

## License

The code in this repository is licensed under
[0BSD](https://spdx.org/licenses/0BSD.html), a
[“public-domain equivalent”](https://en.wikipedia.org/wiki/Public-domain-equivalent_license)
license. See [**`LICENSE`**](LICENSE).

## Usage

A C compiler and `make` are needed.

Run `./run-experiment`, or experiment interactively by running `./sig` directly
(you may need to kill the subprocesses yourself).

The `sig` command accepts a signal number as an argument. For example, to make
it use `SIGKILL` instead of its default of `SIGTERM`, run:

```sh
./sig 9
```

Or to have the `run-experiment` script call `sig` that way:

```sh
./run-experiment 9
```
