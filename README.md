# SpyC - another C/C++ static analyzer

[![CircleCI](https://circleci.com/gh/mpsm/spyc.svg?style=shield)](https://circleci.com/gh/mpsm/spyc)
[![codecov](https://codecov.io/gh/mpsm/spyc/branch/master/graph/badge.svg)](https://codecov.io/gh/mpsm/spyc)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/62d44bdc41c34f4bb701d27b46ab93f3)](https://www.codacy.com/manual/mpsm/spyc?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=mpsm/spyc&amp;utm_campaign=Badge_Grade)

SpyC is an application which main purpose is to help developer
analyze complex C codebase. Its very first version allows to
scan C code looking for function definitions and calls and output
results as a digraph in dot format.

## Building

### Prerequisites

In order to build SpyC, you need to install first:
*   cmake 3.10
*   C++ compiler: GCC8 or Clang9
*   LLVM and clang development libraries (version `9.0`) with dependencies
*   Googletest if running unittests
For a complete reference see [Dockerfile](Dockerfile).

### Building with `cmake`

To build project clone repo and simply run:
```bash
cmake -B build .
cmake --build build
```
It will create `spyc` binary in the `build/src` subdirectory.

### Customizing `cmake` builds

Apart of standard cmake variables, following variables changes `cmake` default behavior:
*   `Clang_HINT` - specify Clangs sysroot,
*   `GTest_HINT` - specify GTest sysroot,
*   `SPYC_TESTS=ON` - turn on building unittests.

## Running

SpyC uses same invocation like other LLVM tools. Basic usage:
```bash
spyc <soucefile> -- 
```
It is possible to process multiple source files at once:
```bash
spyc <soucefile1> <sourcefile2> ... <sourcefileN> -- 
```
To add compiler flags specify them after `--` or use a compilation database file:
```bash
spyc <sourcefile> -- -I /usr/local/include
spyc <sourcefile> compilation_database.json
```

### Diagram types

It is possible to generate diagram with either all calls found during code
parsing (default) or a graph generated starting from specified method using
`--diagram-type=graph` option. Default starting point is `main`, which can
be changed with `--graph-start` option.

### Example

Generate and display callgraph in PNG format (`graphviz` is required):
```bash
./build/spyc test/basic.c -- | dot -Tpng > basic.png && eog basic.png
```
![](doc/img/basic.png)
