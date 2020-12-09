# Equipment Registration System

## Description

This is a system for recording equipment and alerts related to them. For more details see the [specification file]('./../specification_files/Firmware%20-%20Especificação%20Prova%20Prática%20Programador.pdf).

## Prerequisites

This program was developed in a Ubuntu - 20.04.1 LTS running on a WSL - V2 inside a Windows 10, using has a compiler the gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0. But **probably works on any Unix-based system with a modern GCC**.

## Compilation

```bash
gcc registration_system.c -o registration_system
```

## Usage

```bash
./registration_system
```

## Observations

* The maximum length of any string was defined as 50 chars, if that is not enough that can be changed in the `#define MAX_STRINGS_LEN 50`
* Since the text files are formatted as csv, there should be no commas in any of the fields.