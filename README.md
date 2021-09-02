![Microsoft Defending Democracy Program: ElectionGuard Python][banner image]

# 🗳 ElectionGuard C++

![Github Package Action](https://github.com/microsoft/electionguard-cpp/workflows/Release/badge.svg) [![license](https://img.shields.io/github/license/microsoft/electionguard-cpp)](https://github.com/microsoft/electionguard-cpp/blob/main/LICENSE) [![license](https://img.shields.io/nuget/v/Electionguard.Encryption)](https://www.nuget.org/packages/ElectionGuard.Encryption/)

This repository is a "reference implementation" of an ElectionGuard ballot encryption library written in c++ and includes a C-compatible API for referencing the library from pure-c application. This core SDK performs ballot encryption and verification functions and is suitable for execution on voting system hardware and low powered devices. It is designed to be integrated into existing (or new) voting system software.

This repository is `pre-release` software to showcase the ElectionGuard API implemented in a native language. It is not feature complete and should not be used for production applications.

## 📁 In This Repository

| File/folder       | Description                                |
| ----------------- | ------------------------------------------ |
| `.github`         | Github workflows and issue templates       |
| `.vscode`         | VS Code configurations                     |
| `/bindings`       | Binding interfaces for different languages |
| `/cmake`          | `CMake` dependencies`                      |
| `/include`        | Public include headers                     |
| `/src`            | ElectionGuard source code                  |
| `/test`           | Unit tests                                 |
| `.clang-format`   | Style guidelines                           |
| `.gitignore`      | Define what to ignore at commit time.      |
| `CmakeLists.txt`  | Root CMake file                            |
| `CHANGELOG.md`    | List of changes to the sample.             |
| `CONTRIBUTING.md` | Guidelines for contributing to the sample. |
| `README.md`       | This README file.                          |
| `LICENSE`         | The license for the sample.                |

## ❓ What Is ElectionGuard?

ElectionGuard is an open source software development kit (SDK) that makes voting more secure, transparent and accessible. The ElectionGuard SDK leverages homomorphic encryption to ensure that votes recorded by electronic systems of any type remain encrypted, secure, and secret. Meanwhile, ElectionGuard also allows verifiable and accurate tallying of ballots by any 3rd party organization without compromising secrecy or security.

Learn More in the [ElectionGuard Repository](https://github.com/microsoft/electionguard)

## 🦸 How Can I use ElectionGuard?

ElectionGuard supports a variety of use cases. The Primary use case is to generate verifiable end-to-end (E2E) encrypted elections. The ElectionGuard process can also be used for other use cases such as privacy enhanced risk-limiting audits (RLAs). This implementation only includes encryption functions and cannot be used to generate election keys and it cannot decrypt tally results.

This c++ implementation also includes a C API that can be consumed from anywhere that can call C code directly. A .Net Standard package is also provided.

## 💻 Requirements

### All Platforms

- A [C++17](https://isocpp.org/get-started) standard compliant compiler is required to build the core library. While any modern compiler should work, the library is tested on a subset. Check out the [GitHub actions](#) to see what is officially supported.
- [GNU Make](https://www.gnu.org/software/make/manual/make.html) is used to simplify the commands and GitHub Actions. This approach is recommended to simplify the command line experience. This is built in for MacOS and Linux. For Windows, setup is simpler with [Chocolatey](https://chocolatey.org/install) and installing the provided [make package](https://chocolatey.org/packages/make). The other Windows option is [manually installing make](http://gnuwin32.sourceforge.net/packages/make.htm).
- [CMake](https://cmake.org/) is used to simplify the build experience.

### 🤖 Android

To build for android, you need the Android SDK and platforms 21 and 26. The easiest way is to download android studio. Alternatively, you can use the SDK installation that ships with the Xamarin Tooling in Visual Studio. WE also require the use of the Android NDK. Android builds can be compiled on Linux, Mac, or Windows

- [Android SDK](https://developer.android.com/studio/#downloads)
- [SDK 21](https://developer.android.com/studio/releases/platforms#5.0)
- [SDK 26](https://developer.android.com/studio/releases/platforms#8.0)
- [NDK 21](https://developer.android.com/ndk/downloads/)

### 🍏 iOS

To build for iOS you need XCode installed

- [XCode](https://developer.apple.com/xcode/resources/) and the [Command Line Tools for XCode](#)
- [CMake 3.19](https://cmake.org/) may be necessary, along with changes to the Makefile. [See ISSUE #138](https://github.com/microsoft/electionguard-cpp/issues/138)

### Linux

The automated install of dependencies is currently only supported on debian-based systems. See the makefile for more information.

### 🖥️ Windows (using MSVC)

Building on windows is supported using the `MSVC` toolchain.

- Install [Chocolatey](https://chocolatey.org/install)
- Install [Powershell Core](https://github.com/powershell/powershell)
- Install [VS 2019](https://visualstudio.microsoft.com/vs/)
- Open the Visual Studio Installer and install
  -- MSVC v142 - VS 2019 C++ x64/x86 build tools
  -- Windows 10 SDK (latest)
  -- C++ CMake tools for Windows
  -- C++/CLI support for v142 build tools

### 🖥️ Windows (using MSYS2)

Building on windows is supported using the `MSYS2` toolchain. MSYS is the default toolchain on Windows.

- Install [Chocolatey](https://chocolatey.org/install)
- Install [Powershell Core](https://github.com/powershell/powershell)
- Install [MSYS2](https://www.msys2.org)
- Open the MSYS2 prompt by running the newly-created "MSYS2 MSYS" shortcut in your start menu.
- Inside the prompt, run `pacman -Syu`, then close the window when it prompts you to.
- Reopen the MSYS2 prompt and run:
  ```
  pacman -Syu
  pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake make
  ```
- Modify your `%Path%` to include the newly-installed software. You should include these two paths:
  ```
  C:\msys64\mingw64\bin
  C:\msys64\usr\bin
  ```

#### 🚧 The Procedure Entry Point Could not be Located

When compiling with shared libraries, you may encounter an error running the unit tests project. This is likely due to windows resolving the incorrect implementation of `libstdc++-6.dll`. Solving this depends on your use case, but you can either ensure that the path modifications made above appear before any other paths which include this library (e.g. c\Windows\System32\), or you can include a copy of the correct DLL in the output folder. [See this StackOverflow post for more information](https://stackoverflow.com/questions/18668003/the-procedure-entry-point-gxx-personality-v0-could-not-be-located)

### 🌐 .NET Standard

A .NET Standard binding library is provided so that this package can be consumed from C# applications. At this time, MacOS, Linux and Windows are supported.

- [Visual Studio](https://visualstudio.microsoft.com)
- [NuGet Command Line (CLI)](https://docs.microsoft.com/en-us/nuget/reference/nuget-exe-cli-reference#macoslinux)

## Build C++

Using **make**,

### Download Dependencies

```sh
make environment
```

### Build the Library for the current host (Release, default toolchain)

```sh
make build
```

### Build the Library for the current host (Debug, default toolchain)

```sh
export TARGET=Debug && make build
```

### Android

The Android Build currently Targets API Level 26 but can be configured by modifying the Makefile

Set the path to the NDK, replacing the version with your own

```sh
export NDK_PATH=/Users/$USER/Library/Android/sdk/ndk/21.3.6528147 && make build-android
```

### iOS

The iOS build currently targets iPhone OS 12 but can be configured by modifying the Makefile

Creates a fat binary for the simulator and targets a recent version of iOS

```sh
make build-ios
```

### Windows

Using the default MSYS2 toolchain:

```pwsh
make build
```

Using the MSVC toolchain:

```pwsh
make build-msvc
```

## Build Wrappers

### .Net Standard 2.0

Wraps the build artifacts in a C# wrapper conforming to .Net Standard 2.0.

```sh
make build-netstandard
```

### .Net Framework v4.8

Wraps the MSVC build artifacts in a C++/CLI wrapper to be consumed from a .Net Framework 4.8 application (windows desktop only). Only works on Windows.

```pwsh
// ensure the windows msvc binaries are built
make build-msvc
```

open `./bindings/netframework//ElectionGuard.NetFramework/ElectionGuard.NetFramework.XXX.sln` in VS 2019 and build.

#### Running the .Net Framework Benchmark

Open the NetFramework solution and run the `ElectionGuard.Bench` project

## Test

### Running the C++ and C tests

```sh
make test
```

#### Running the tests on Windows using the MSVC toolchain

```sh
make test-msvc
```

### Running the netstandard tests

You can run the .Net Standard tests using the Xamarin Test runner on the iOS simulator.

```sh
# ensure the iOS and Android binaries are built
make build-netstandard
```

Then, open Visual studio for Mac and run the `ElectionGuard.Tests.iOS` project.

## 📄 Documentation

## Contributing

This project encourages community contributions for development, testing, documentation, code review, and performance analysis, etc. For more information on how to contribute, see [the contribution guidelines][contributing]

### Code of Conduct

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

### Reporting Issues

Please report any bugs, feature requests, or enhancements using the [GitHub Issue Tracker](https://github.com/microsoft/electionguard-python/issues). Please do not report any security vulnerabilities using the Issue Tracker. Instead, please report them to the Microsoft Security Response Center (MSRC) at [https://msrc.microsoft.com/create-report](https://msrc.microsoft.com/create-report). See the [Security Documentation][security] for more information.

### Have Questions?

ElectionGuard would love for you to ask questions out in the open using GitHub Discussions. If you really want to email the ElectionGuard team, reach out at electionguard@microsoft.com.

## License

This repository is licensed under the [MIT License]

## Thanks! 🎉

A huge thank you to those who helped to contribute to this project so far, including:

**[Josh Benaloh _(Microsoft)_](https://www.microsoft.com/en-us/research/people/benaloh/)**

<a href="https://www.microsoft.com/en-us/research/people/benaloh/"><img src="https://www.microsoft.com/en-us/research/wp-content/uploads/2016/09/avatar_user__1473484671-180x180.jpg" title="Josh Benaloh" width="80" height="80"></a>

**[Keith Fung](https://github.com/keithrfung) [_(InfernoRed Technology)_](https://infernored.com/)**

<a href="https://github.com/keithrfung"><img src="https://avatars2.githubusercontent.com/u/10125297?v=4" title="keithrfung" width="80" height="80"></a>

**[Matt Wilhelm](https://github.com/AddressXception) [_(InfernoRed Technology)_](https://infernored.com/)**

<a href="https://github.com/AddressXception"><img src="https://avatars0.githubusercontent.com/u/6232853?s=460&u=8fec95386acad6109ad71a2aad2d097b607ebd6a&v=4" title="AddressXception" width="80" height="80"></a>

**[Dan S. Wallach](https://www.cs.rice.edu/~dwallach/) [_(Rice University)_](https://www.rice.edu/)**

<a href="https://www.cs.rice.edu/~dwallach/"><img src="https://avatars2.githubusercontent.com/u/743029?v=4" title="danwallach" width="80" height="80"></a>

**[Marina Polubelova](https://polubelova.github.io/) [_(INRIA Paris)_](https://prosecco.gforge.inria.fr/)**

<a href="https://polubelova.github.io/"><img src="https://polubelova.github.io/authors/admin/avatar_hu562f921c0165de84dfdc53044b574fa1_846381_270x270_fill_q90_lanczos_center.jpg" title="polubelova" width="80" height="80"></a>

**[Santiago Zanella-Béguelin](https://www.microsoft.com/en-us/research/people/santiago/) [_(Microsoft Research)_](https://www.microsoft.com/en-us/research/)**

<a href="https://www.microsoft.com/en-us/research/people/santiago/"><img src="https://www.microsoft.com/en-us/research/uploads/prod/2020/08/profile_cropped-5f44d9b09ecd7.jpg" title="santiago" width="80" height="80"></a>

**[Jonathan Protzenko](https://jonathan.protzenko.fr/) [_(Microsoft Research)_](https://www.microsoft.com/en-us/research/group/research-software-engineering-rise/)**

<a href="https://jonathan.protzenko.fr/"><img src="https://jonathan.protzenko.fr/assets/protzenko.jpg" title="protzenko" width="80" height="80"></a>

<!-- Links -->

[banner image]: https://raw.githubusercontent.com/microsoft/electionguard-python/main/images/electionguard-banner.svg
[pull request workflow]: https://github.com/microsoft/electionguard-ccpp/blob/main/.github/workflows/pull_request.yml
[contributing]: https://github.com/microsoft/electionguard-cpp/blob/main/CONTRIBUTING.md
[security]: https://github.com/microsoft/electionguard-cpp/blob/main/SECURITY.md
[mit license]: https://github.com/microsoft/electionguard-cpp/blob/main/LICENSE
