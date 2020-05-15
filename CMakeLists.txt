cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

set(META_PROJECT_NAME        "ElectionGuard Core")
set(META_PROJECT_EXPORT      "ElectionGuard")
set(META_PROJECT_TARGET      "electionguard")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "1")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")

set(LIBRARY_PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

project(${META_PROJECT_NAME}
  VERSION ${META_VERSION}
  LANGUAGES C CXX
)

# Create a version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_PROJECT_NAME} v${META_VERSION}")

# ---- Execution guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new /build/ directory and run CMake from there.")
endif()

# ---- Options ----

option(CMAKE_BUILD_TYPE         "Build with or without debug symbols"                       Release)
option(BUILD_SHARED_LIBS         "Build SHARED libraries"                                   OFF)
option(OPTION_ENABLE_TESTS      "Enable support for testing private headers"                OFF)
option(CODE_COVERAGE            "Enable code coverage"                                      OFF)
option(OPTION_GENERATE_DOCS     "Generate documentation"                                    OFF)
option(USE_STATIC_ANALYSIS      "use static analysis tools"                                 OFF)
option(USE_DYNAMIC_ANALYSIS      "use dynamic analysis tools"                               OFF)
option(USE_FORMATTING           "use formatting tools"                                      ON)

# ---- Modules ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---- Dependencies ----

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME StableCoder-cmake-scripts
    GITHUB_REPOSITORY StableCoder/cmake-scripts
    GIT_TAG 3a469d8251660a97dbf9e0afff0a242965d40277
)

# ---- Enable Tools ----

# available values are: Address, Memory, MemoryWithOrigins, Undefined, Thread, Leak, 'Address;Undefined'
if (USE_SANITIZER)
    message("++ Using Sanitizer: ${USE_SANITIZER}")
    include(${StableCoder-cmake-scripts_SOURCE_DIR}/sanitizers.cmake)
endif()

if (CODE_COVERAGE)
    message("++ Running with coverage")
    include(${StableCoder-cmake-scripts_SOURCE_DIR}/code-coverage.cmake)
    add_code_coverage()
endif()

if (USE_STATIC_ANALYSIS)
    message("++ Running with static analysis")
    set(CLANG_TIDY ON)
    set(CPPCHECK ON)
    include(${StableCoder-cmake-scripts_SOURCE_DIR}/tools.cmake)
    clang_tidy()
    cppcheck()
endif()

if (USE_FORMATTING)
    message("++ Running with formatting")
    include(${StableCoder-cmake-scripts_SOURCE_DIR}/formatting.cmake)
endif()

# --- Properties ---

set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)

# IDE's usually prefer folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Set a DEBUG definition
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("++ Setting DEBUG during compile")
    add_compile_definitions(DEBUG)
endif()

# ---- Sources ----

add_subdirectory(src)

if (OPTION_ENABLE_TESTS)
    add_subdirectory(test)
endif()

if (OPTION_GENERATE_DOCS)
    add_subdirectory(docs)
endif()
