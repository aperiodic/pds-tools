# PDS Tools

This is a C tool that parses NASA [Planetary Data Science][nasa-pds] (PDS) files into an abstract syntax tree for easy machine manipulation.
It also has Java Native Interface bindings.

I wrote this because I couldn't find any open-source code to do this (NASA releases a compiled shared library to do this sort of thing, but no source), and it seemed like fun.

This project is still quite new and hasn't been extensively tested on extant PDS files.
If you find a PDS label that doesn't parse correctly, or causes errors, please [open an issue about it][issues], [email me][email], or tweet @aperiodic.

[nasa-pds]: http://pds.jpl.nasa.gov/
[issues]: https://github.com/aperiodic/pds-tools/issues
[email]: mailto:dlp@aperiodic.org

## Usage - C Tool

The only way to use this project at the moment is to clone this repo and compile it yourself.

```sh
git clone https://github.com/aperiodic/pds-tools

cd pds-tools
make && make demo && ./bin/pds-parse-demo samples/mola.lbl
```

The default make target produces a shared library named `libpdstools.so` in the `bin` folder.
The `demo` target produces `bin/pds-parse-demo`, which can be run with the path to a PDS file in order to parse its label and print some basic information about it.

## Usage - Java Bindings

The Java bindings live in `bindings/java`.

```sh
cd bindings/java
make && make run label=../../samples/mola.lbl
```

The `make` command for the Java bindings produces two things:
  * a `libpdstools.so` native shared library that will need to be in your `java.library.path`.
  * a `PDSTools.jar` JAR that contains a Java wrapper around the native library.
You may want to look at the definition of the `run` target in the Java bindings' Makefile for how exactly to run a simple demo of the Java bindings.

That demo also serves as documentation on the Java bindings.
The `bindings/java/org/aperiodic/PDSTools.java` file contains a main method that parses a PDS label file, then prints its version, metadata, and embedded objects (and their attributes).

## Debugging

The `debug` make target can be used to turn on print statements emitted during parsing that can be helpful when debugging errors.

```sh
make debug && ./build/pds-parse samples/mola.lbl
```
