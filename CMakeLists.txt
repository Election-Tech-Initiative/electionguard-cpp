cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

set(META_PROJECT_NAME        "ElectionGuard")
set(META_PROJECT_EXPORT      "ElectionGuard")
set(META_PROJECT_TARGET      "electionguard")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "3")
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

option(CMAKE_BUILD_TYPE          "Build with or without debug symbols"                       Release)
option(BUILD_SHARED_LIBS         "Build SHARED libraries"                                    OFF)
option(EXPORT_INTERNALS          "Export Internal Headers"                                   OFF)
option(CAN_USE_VECTOR_INTRINSICS "Use vector intrinsics for math functions if available"     OFF)
option(USE_32BIT_MATH            "Use the 32 bit optimized math impl"                        OFF)
option(USE_TEST_PRIMES           "Use the smaller test primes (do not use in prod)"          OFF)
option(OPTION_ENABLE_TESTS       "Enable support for testing private headers"                OFF)
option(TEST_SPEC_VERSION         "Use this spec version for tests"                           "0.95.0")
option(TEST_USE_SAMPLE           "the sample to use, full, hamilton-general, minimal, small" "hamilton-general")
option(CODE_COVERAGE             "Use code coverage"                                         OFF)
option(OPTION_GENERATE_DOCS      "Generate documentation"                                    OFF)
option(USE_DYNAMIC_ANALYSIS      "Enable Dynamic tools"                                      OFF)

# ---- Modules ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---- Dependencies ----

set(CPM_DOWNLOAD_VERSION 0.31.0) 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})


# ---- Tools ----

include(cmake/tools.cmake)

# --- Properties ---

set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            OFF)

set(CMAKE_THREAD_PREFER_PTHREAD     TRUE)
set(THREADS_PREFER_PTHREAD_FLAG     TRUE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# IDE's usually prefer folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")

# Set a DEBUG definition
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("++ Setting DEBUG during compile")
    add_compile_definitions(DEBUG)
    add_compile_definitions(LOG_DEBUG)
    add_compile_definitions(JSON_DIAGNOSTICS=1)
endif()

# Allow explicitly setting debug logs in release builds
if (LOG_LEVEL MATCHES debug)
    message("++ Using DEBUG Logs")
    add_compile_definitions(LOG_DEBUG)
elseif(LOG_LEVEL MATCHES trace)
    message("++ Using TRACE Logs")
    add_compile_definitions(LOG_TRACE)
endif()

if(EXPORT_INTERNALS)
    message("++ Exporting Internal Headers")
    add_compile_definitions(EXPORT_INTERNALS)
endif()

# HACL includes processor optimized vectors where possible
if(CAN_USE_VECTOR_INTRINSICS)
    message("++ Using Vector Intrinsics")
    add_compile_definitions(HACL_CAN_COMPILE_VEC128)
    add_compile_definitions(HACL_CAN_COMPILE_VEC256)
endif()

if(USE_TEST_PRIMES)
    message("++ Using Test Primes. Do not use in production.")
    add_compile_definitions(USE_TEST_PRIMES)
else()
    add_compile_definitions(USE_STANDARD_PRIMES)
endif()

# HACK: Disable explicit bzero on android
# TODO: define explicit secure zero on android and re-enable
if(DEFINED CMAKE_ANDROID_ARCH_ABI)
    add_compile_definitions(LINUX_NO_EXPLICIT_BZERO)
endif()

# ---- Sources ----

add_subdirectory(src)

if (OPTION_ENABLE_TESTS)
    # TODO: move mocks to their own folder
    add_subdirectory(test)
endif()

if (OPTION_GENERATE_DOCS)
    add_subdirectory(docs)
endif()
