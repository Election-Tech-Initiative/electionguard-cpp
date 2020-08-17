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
option(OPTION_GENERATE_DOCS     "Generate documentation"                                    OFF)

# ---- Modules ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---- Dependencies ----

# ---- Tools ----

include(cmake/tools.cmake)

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

# HACK: Disable x86 intrinsics
# TODO: load the appropriate intrinsics for the processor architecture
# this is currently only being set when the platform is explicitly set (such as on ios)
if(DEFINED PLATFORM)
    add_compile_definitions(BROKEN_INTRINSICS)
endif()

# ---- Sources ----

add_subdirectory(src)

if (OPTION_ENABLE_TESTS)
    add_subdirectory(test)
endif()

if (OPTION_GENERATE_DOCS)
    add_subdirectory(docs)
endif()
