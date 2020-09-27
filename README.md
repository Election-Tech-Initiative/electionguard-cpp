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

- [C++17](https://isocpp.org/get-started) standard.  a `C++17` compliant compiler is required to build the core library.

### CMake 

- [CMake 3.14](https://cmake.org/) as it's build system.

### Android (optional)

- [Android SDK](https://developer.android.com/studio/#downloads)
- [NDK 21](https://developer.android.com/ndk/downloads/)

### iOS (optional)

- [XCode](https://developer.apple.com/xcode/resources/)
- [Command Line Tools for XCode](#)

### Xamarin (optional)

- [Visual Studio for Mac](https://visualstudio.microsoft.com/vs/mac/)
- [NuGet Command Line (CLI)](https://docs.microsoft.com/en-us/nuget/reference/nuget-exe-cli-reference#macoslinux)

### make (optional)

**make** is used to simplify the build commands and GitHub Actions. This is built in for MacOS and Linux, but Windows installation instructions can be found [here](http://gnuwin32.sourceforge.net/packages/make.htm).

## Quick Start

Using **make**,

### To Downloiad Dependencies

```sh
make environment
```

### Build the Library for the current host (Release)

```sh
make build
```

### Build the Library for the current host (Debug)

```sh
export BUILD_DEBUG=true && make build
```

### Build for android

Set the path to the NDK, replacing the version with your own

```sh
export NDK_PATH=/Users/$USER/Library/Android/sdk/ndk/21.3.6528147 && make build-android
```

### Build for iOS

Creates a fat binary for the simulator and targets a recent version of ios

```sh
make build-ios
```

### Build for Xamarin

Wraps the android and ios build artifacts in a NuGet packge to be consumed from a Xamarin application (classic or forms)

```sh
make build-netstandard
```

## Test

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

### Execute the Demo in Xamarin Forms

```sh
make build-netstandard
```

To consume the local .nupkg, you must configure Visual Studio to load packages from a local repository.
1. [Add a Local NuGet Feed to Visual Studio](https://docs.microsoft.com/en-us/xamarin/cross-platform/cpp/#adding-the-local-nuget-feed-to-the-nuget-configuration)
2. specify the nuget output directory as the package feed: (`./build/bindings/netstandard/`)

Open `Visual Studio` and open the Xamarin Forms demo at `./apps/demo_in_xamarin/ElectionGuardCore.sln`

Then, build.

### Execute the Demo in Swift

```sh
make build-ios
```
You need to copy the resulting binary to the swift demo folder.

```sh
cp ./build/electionguard-core/libs/ios/lib/libelectionguard.a ./apps/demo_in_swift/ElectionGuardDemo/ElectionGuardDemo/libelectionguard.a
```

Then, open `./apps/demo_in_swift/ElectionGuardDemo.xcodeproj` in xcode and run.

## Key concepts

🚧Under Construction
