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

# ---- Modules ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---- OPTIONS ----

option(CMAKE_BUILD_TYPE         "Build with or withoutdebug symbols"                        Release)
option(OPTION_ENABLE_TESTS      "Enable support for testing private headers"                OFF)

# --- PROPERTIES ---

# IDE's usually prefer folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Set a DEBUG definition
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("setting DEBUG during compile")
    add_compile_definitions(DEBUG)
endif()

# ---- Library Sources ----

add_subdirectory(src)
