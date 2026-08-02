# C_Cli

## Description

> [!IMPORTANT]
> A detailed description is written in [c_cli.h](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h).

C_Cli is header only heapless typed cli library written in C99.
It's designed to be easily integrated in pre existing Cli objects and with pre existing 
flag parsing logic.

## Features

> [!IMPORTANT]
> A detailed lists of features is written in [c_cli.h](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h).

Below a list of a few of the available features out of the box:
- heapless                  : no heap allocations
- auto alignement           : the cli will be automatically aligned based on the flags that compose the cli
- typed arg flags           : each argument of each flag has a type which is checked by the library
- long and short flag       : each flag has a long and a short version
- base flags                : --help/-h, --verbose/-v are already defined by the library
- user default flag         : optional possibility to define custom default flags.
                              Default is none and triggers printing help on stderr.
- name detection            : the cli's name will be equivalent of the program that is using it
- invalid input flag        : invalid user flags are auto detected and
                              printed as warning to the screen

## Usage

> [!IMPORTANT]
> A detailed guide on how to use the library features is written in [c_cli.h](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h).

To use the library you need to manually define the following elements:

- an option object with type struct CCliUserArgs
- an array of CCliArgDef which tells the library the elements which defines the cli
- a set of functions, required in the definition an CCliArgDef to handle each flag

## Examples

Examples are available in the [tests](https://github.com/mrmonopoly-cyber/c_cli/tree/master/tests) directory
and in the main header [c_cli.h](https://github.com/mrmonopoly-cyber/c_cli/blob/master/c_cli/c_cli.h)
