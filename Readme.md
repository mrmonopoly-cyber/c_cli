# C_Cli

## Description

C_Cli is a **header-only, heapless, typed CLI library written in C99**.

It is designed to be easily integrated into **pre-existing CLI objects and flag parsing logic**, without requiring a complete rewrite of the existing CLI implementation.

> [!IMPORTANT]
> The detailed API reference and library documentation are available in [`c_cli.h`](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h).

## Features

The following features are available out of the box:

* **Heapless** — no dynamic memory allocation.
* **Automatic alignment** — CLI help output is automatically aligned based on the defined flags.
* **Typed arguments** — each flag argument has an associated type that is validated by the library.
* **Long and short flags** — flags can have both long and short forms.
* **Built-in flags** — `--help/-h` and `--verbose/-v` are provided by the library.
* **Default flags** — optionally define custom default behavior when no recognized user flags are provided.
* **Program name detection** — the CLI name is automatically detected from the program invocation.
* **Invalid flag detection** — unknown flags are automatically detected and reported as warnings.
* **Argument lists** — flags can accept comma-separated argument lists.

> [!IMPORTANT]
> The complete list of features and their configuration options is documented in [`c_cli.h`](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h).

## Usage

C_Cli is designed to be integrated into an existing CLI implementation.

To use the library, you need to define:

* a user argument structure of type `CCliUserArgs`;
* an array of `CCliArgDef` describing the CLI flags and their arguments;
* parser functions used by the flag definitions to process their arguments.

The library is then integrated by including `c_cli.h` once for the declarations and configuration, and once with `CCLI_IMPLEMENTATION` defined to deploy the implementation.

> [!IMPORTANT]
> The detailed usage guide and API documentation are available in [`c_cli.h`](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h).

## Examples

Complete examples are available in the [`tests`](https://github.com/mrmonopoly-cyber/c_cli/tree/master/tests) directory.

The main header [`c_cli.h`](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h) also contains usage examples and API documentation.

