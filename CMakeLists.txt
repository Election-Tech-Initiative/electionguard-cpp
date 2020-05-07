cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

set(META_PROJECT_NAME        "ElectionGuard Core")
set(META_PROJECT_TARGET      "ElectionGuard")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "1")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")

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

# ---- Dependencies ----


# ---- OPTIONS ----

option(BUILD_SHARED_LIBS        "Build shared instead of static libraries."                 ON)
option(CMAKE_BUILD_TYPE         "Build with debug symbols"                                  Debug)
option(OPTION_SELF_CONTAINED    "Create a self-contained install with all dependencies."    OFF)
option(OPTION_BUILD_TESTS       "Build tests."                                              ON)
option(OPTION_BUILD_DOCS        "Build documentation."                                      OFF)
option(OPTION_BUILD_EXAMPLES    "Build examples."                                           OFF)
option(OPTION_ENABLE_COVERAGE   "Add coverage information."                                 OFF)

# --- PROPERTIES ---

# IDE's usually prefer folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Set a DEBUG definition
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("setting DEBUG during compile")
    add_compile_definitions(DEBUG)
ENDIF()

# ---- Library Sources ----

add_library(electionguard
    ${PROJECT_SOURCE_DIR}/src/electionguard/encrypt_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/encrypt.cpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/encrypt.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/encrypt.hpp
)

# ---- Compile ----

# Require c++14, this is better than setting CMAKE_CXX_STANDARD 
# since it won't pollute other targets
# note : cxx_std_* features were added in CMake 3.8.2
target_compile_features(electionguard PRIVATE cxx_std_17)

# Set the public include directory depending on 
# if the target is being exported or installed
target_include_directories(electionguard
    SYSTEM PUBLIC
        $<INSTALL_INTERFACE:include/${META_PROJECT_TARGET}/${PROJECT_VERSION}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/electionguard
)

# ---- Install ----

# Install the electionguard library in the default location, and associate
# electionguard with the ElectionGuard export
install(
    TARGETS electionguard 
    EXPORT ${META_PROJECT_TARGET}
)

# Install public header files
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/${META_PROJECT_TARGET}/${PROJECT_VERSION}
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h*"
)

# Generate the build-tree ElectionGuardConfig.cmake for use in other cmake
# projects without needing to install
export(
    EXPORT ${META_PROJECT_TARGET}
    FILE "${PROJECT_BINARY_DIR}/${META_PROJECT_TARGET}Config.cmake"
)

# Generate the install-tree ElectionGuardConfig.cmake for use in other cmake
# projects after this library has been installed
install(
    EXPORT ${META_PROJECT_TARGET}
    FILE ${META_PROJECT_TARGET}Config.cmake
    DESTINATION lib/cmake/${META_PROJECT_TARGET}/${PROJECT_VERSION}
)

# ---- Executable ----