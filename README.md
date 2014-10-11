# PDS Tools

This is a C shared library that parses NASA [Planetary Data Science][nasa-pds] (PDS) files into structs with hashmap access to metadata and embedded objects.
It also has Java Native Interface bindings.

The library is currently only distributed via git.
Its only dependency, [Christopher Clark's hashtable library][cc-hash], is checked in to the `lib` folder, so it can be compiled as long as you have a C compiler and runtime.

[cc-hash]: https://github.com/ryantenney/chashtable


I wrote this because I couldn't find any open-source code to do this (NASA releases a compiled shared library to do this sort of thing, but no source), and it seemed like fun.

This project is still quite new and hasn't been extensively tested on extant PDS files.
If you find a PDS label that doesn't parse correctly, or causes errors, please [open an issue about it][issues], [email me][email], or tweet @aperiodic.

[nasa-pds]: http://pds.jpl.nasa.gov/
[issues]: https://github.com/aperiodic/pds-tools/issues
[email]: mailto:dlp@aperiodic.org

## Usage - C Tool

To get started:

```sh
git clone https://github.com/aperiodic/pds-tools

cd pds-tools
make run
```

The default make target produces a shared library named `libpdstools.so` in the `bin` folder.
The `demo` target produces `bin/pds-parse-demo`, and can be run on an included PDS label by executing the run target with `make run`.

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

## License

This library's source and its bindings are licensed under the GPLv3 (see `COPYING`).
The included hash table library from Christopher Clark is included in `lib` under the terms of the MIT License; the GPLv3 license that applies to the rest of this repository does not include the contents of the `lib` directory.
