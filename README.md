---
page_type: sample
languages:
- cplusplus
description: "ElectionGuard Core Library"
urlFragment: "https://github.com/microsoft/electionguard-core"
---

# ElectionGuard Core

<!-- 
Guidelines on README format: https://review.docs.microsoft.com/help/onboard/admin/samples/concepts/readme-template?branch=master

Guidance on onboarding samples to docs.microsoft.com/samples: https://review.docs.microsoft.com/help/onboard/admin/samples/process/onboarding?branch=master

Taxonomies for products and languages: https://review.docs.microsoft.com/new-hope/information-architecture/metadata/taxonomies?branch=master
-->

This repository is a "reference implementation" of an ElectionGuard ballot encryption device written in c++.  This core SDK performs ballot encryption and verification functions and is suitable for execution on voting system hardware.  It is designed to be integrated into existing (or new) voting system software.  The `ElectionGuard Core` SDK is written in C++ and includes a C-compatible API for referencing the library from pure-c applications.

This repository is `pre-release` software to showcase the ElectionGuard API implemented in a native language.

## Contents

Outline the file contents of the repository. It helps users navigate the codebase, build configuration and any related assets.

| File/folder       | Description                                |
|-------------------|--------------------------------------------|
| `.vscode`         | VS Code configurations                     |
| `/apps`           | Example applications in `C` and `C++`      |
| `/cmake`          | `CMake` dependencies`                      |
| `/include`        | Public include headers                     |
| `/src`            | Electionguard source code                  |
| `/test`           | Unit tests                                 |
| `.clang-format`   | Style guidelines                           |
| `.gitignore`      | Define what to ignore at commit time.      |
| `CmakeLists.txt`  | Root CMake file                            |
| `CHANGELOG.md`    | List of changes to the sample.             |
| `CONTRIBUTING.md` | Guidelines for contributing to the sample. |
| `README.md`       | This README file.                          |
| `LICENSE`         | The license for the sample.                |

## Prerequisites

### C++17

This code builds against the [C++17](https://isocpp.org/get-started) standard.  a `C++17` compliant compiler is required to build the core library.

### CMake 

This code uses [CMake 3.14](https://cmake.org/) as it's build system.

### GMP

This code depends on [The GNU Multiple Precision Arithmetic Library](https://gmplib.org/#DOWNLOAD) to calculate large integers.

### make (optional)

**make** is used to simplify the build commands and GitHub Actions. This is built in for MacOS and Linux, but Windows installation instructions can be found [here](http://gnuwin32.sourceforge.net/packages/make.htm).

## Quick Start

Using **make**,

### To Downloiad Dependencies

```sh
make environment
```

### Build the Library (Release)

```sh
make build
```

### Build the Library (Debug)

```sh
export BUILD_DEBUG=true && make build
```

### Run the Tests

```sh
make test
```

## Running

### Execute the Demo in C

```sh
make demo-c
```

### Execute the Demo in C++

```sh
make demo-cpp
```

## Key concepts

🚧Under Construction
