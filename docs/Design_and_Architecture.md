# Design & Architecture

This describes the design and architecture of the `electionguard-cpp` project.

## Design

The C++ project only implements a subset of the full ElectionGuard specification.  The C++ implementation is designed solely to support ballot encryption and verification on "lower pwered" hardware.  This library only implements the objects and functions necessary to generate and verify ballots.

### ✅ Cross Platform

One goal of this project is to be able to run on as many platforms as possible.  The CI pipeline is configured across a variety of common platforms and compilers, but if there is something missing, please open an issue.

### ✅ Modern Tools

This project uses C++17 and requires a compiler capable of interpreting the C++17 specification.  A public C API is included to consume the library from C code, but a C++ compiler is still required.

### ✅ Extensibility

The library is intentionally general-purpose to support the different use cases of "end to end verifiable" voting systems. Different projects may wish to use different layers of the library, including math primitives, encryption functions, and more.  The library exposes a C++ Interface as well as a C Interface and both are treated as first-class citizens.  Binding layers are also provided to simplify consuming the library from other languages.

### ✅ Object Oriented Design (OOD) & Functional Methods

An additional goal is to structure the code using familiar object-oriented design patterns while exposing underlying immutable and functional-style methods. Higher-order functions are also included to simplify using the library for common use cases.  This allows users to both construct high-level objects in an OOP fashion and pass them to a single function call (such as `encryptBallot`), or compose a domain-specific object graph and directly call the underlying methods in a functional way (such as `encryptSelection`). 

Class methods are used for simplicity, but sophistication with regard to inheritance, object encapsulation, or design patterns is intentionally avoided. These class methods usually rely on the aforementioned functional methods unless the class contains state.

One notable pattern used is the [Pointer to Implementation Idiom](https://en.cppreference.com/w/cpp/language/pimpl) for object encapsulation to allow the internal business logic to evolve while maintaining stable public API's.

### ✅ Immutable

The library prefers immutable objects where possible to encourage simple data structures.

### ✅ Concurrency

While this library is not yet explicitly guaranteed to be _thread-safe_, it's definitely meant to work properly when the caller wants to run more than one thing at a time. This means there is intentionally minimal global, mutable state in the library.  If global mutable state is added, such as a discrete-log function doing internal memoization, thread-safety will be considered.

### ✅ Exceptions

This library uses exceptions internally, and public C++ API's may throw exceptions.  When consuming the library using the C Interface, return-code enumerations are used to communicate success or failure.

### ✅ Safety

This project attempts to use modern c++ features and tools to catch memory issues at compiletime and during release cycles.  Smart Pointers and move semantics are used throughout to help the consumer understand when the caller is responsible for managing object lifecycles.  The CI/CD pipelines also run static and dynamic analysis to help catch leaks.  Because of the variety of nuance between targets, processor architectures, toolchains, and operating systems, it is possible gaps exist on some platformjs.  Please open an issue if a gap is found.

### 🚫 Multiple Inheritance

Although a handy c++ feature, for implementation simplicity this feature is not used and should be avoided wherever possible.

## Project Practices

### 🚀 Continuous Integration

Github actions are used to execute a variety of tools and tests across a broad spectrum of targets to help maintain cross-platform compatibility.  Because of this, certain targets can only be executed on certain operating systems easily (e.g. iOS).  The CI pipeline runs on every pull request.

### 🧹 Clean Code

The library uses several tools to assist developers in maintaining clean code. Visual Studio Code is recommended for easier setup.

#### Typing

This library uses both complete and opaque types depending on the circumstances.  Where possible, all objects consumed and returned across the C++ API are complete types.  Internally the C++ header files use opaque class types to implement the `pimpl` idiom.  The types used by the C API are completetly opaque.

#### Formatting

[Clang Format](https://clang.llvm.org/docs/ClangFormat.html) is used for auto-formatting and checkiung the formatting of the code.

#### Linting

Linting is handled by compiler flags, clang tidy, and cpp check.  Check out the root `CMakeLists.txt` and `tools.cmake` for more information.

### 🧪 Testing

The goal of the project is 100% code coverage with an understanding that there are some limitations.

## Architecture

### Layout

This project aims to use structural patters similar to the python repository.  While it is not a direct port, many of the interfaces and paradigms found in the python library also apply to this project.

### 📦 Math Library

This project uses the [HACL* Math Library](https://github.com/project-everest/hacl-star) from [Project Everest](https://project-everest.github.io/) for low-level math operations.  The low-level math operations are not exposed across any public API surface at this time.

### Linking

This library is generally consumable as static library or as a dynamic library but there may be some limitations depending on the platform target.  Additionally, dependencies may be static or dynamic depending on the target.

### Object Lifecycle

If you have any questions about consuming objects from this library, please [check or open a discussion](https://github.com/microsoft/electionguard/discussions)

#### C++ API Lifecycle

The C++ API is based on smart pointers that demonstrate intention to the consumer how to consider lifecycle.  Move semantics are used throughout.  All public-facing C++ objects can be constructed directly, however some objects include special `make` functions that are used in a similar fashion to the python library.  Standard library types and primitive types are usually copied when constructing new objects, while domain types are usually moved.  The indended behavior should be well-defined depending if a property is passed by reference or by value.

Effort is given to follow the [Rule of Five](https://cpppatterns.com/patterns/rule-of-five.html) however this is not always guaranteed.

When accessing properties of structures, a _raw pointer_ may be returned to indicate the caller does not own the object (e.g. `ElementModQ *getCryptoHash()`), while a Smart pointer is returned if the object is owned (e.g. `std::unique_ptr<ElementModQ> crypto_hash_with`) by the caller.  The [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) is used when returning vectors of objects owned by the callee (e.g. `std::vector<std::reference_wrapper<PlaintextBallotSelection>> getSelections()`).  

#### C API Lifecycle

Because the C API does not support smart pointers, consideration is given to using `make` functions where an object graph may be composed (analogous to the `make` functions in the C++ API and in python).  Some structures include `new` methods as well to construct primitive objects directly.  C types also generally include `free` methods to release resources.  _Property getter_ methods usually include `get` in the function signature (e.g. `_get_description_hash`). Because the C API is a facade over the C++ API, careful consideration must be given to freeing resources.

Generally:
1. If you built your object graph manually using `new` then you must call `free` on each object you constructed. (such as an array of `eg_selection_description_t[]`).
2. If you built your object graph using `make` or an out paramater was passed by function, you must only call `free` on the top-level object.
3. If the corresponding c++ returns a `unique_ptr` to a domain type then it is owned by the caller and must be freed.
4. If the corresponding C++ returns a standard pointer (`*`) it is owned by the callee and should not be freed.
5. If the corresponding c++ returns a primitive type it is owned by the caller and must be freed.
6. If the corresponding c++ returns a `std::string` then a copy is made using `malloc` and the caller caller must free.

Example:

```c
// A new object is created
eg_ciphertext_election_context_t *result = NULL;
if (eg_ciphertext_election_context_from_json("{some:json}", &result)) {
    // handle error state
}

// A property accessor returns a pointer to a domain type member
eg_element_mod_q_t *crypto_base_hash = NULL;
if (eg_ciphertext_election_context_get_crypto_base_hash(result, &crypto_base_hash)) {
    // handle error state
}

// The property is read out as a null-terminated string
char *crypto_base_hash_hex;
if (eg_element_mod_q_to_hex(crypto_base_hash, &crypto_base_hash_hex)) {
    // handle error state
}

// do stuff...

// Clean Up resources
free(crypto_base_hash_hex);                     // clean up the string that we malloc'd
eg_ciphertext_election_context_free(result);    // clean up the object we created   
eg_element_mod_q_free(crypto_base_hash);        // ERROR! Use after free

```

Refer to the C unit tests for examples.
