cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

project("ElectionGuardTests"
    VERSION 0.0.1 
    LANGUAGES CXX
)

# --- Import tools ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# ---- Options ----

set(CMAKE_BUILD_TYPE        Debug)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
    NAME doctest
    GITHUB_REPOSITORY onqtam/doctest
    GIT_TAG 2.3.7
)

# ---- Sources ----

add_executable(ElectionGuardTests 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hash.cpp
)

target_compile_features(ElectionGuardTests PRIVATE cxx_std_17)

if(MSVC)
    target_compile_definitions(ElectionGuardTests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
endif()

# ---- Link Libraries ----

find_package(GMP REQUIRED)

target_link_libraries(ElectionGuardTests PRIVATE doctest ${META_PROJECT_TARGET} ${GMPXX_LIBRARY})

# ---- Add Tests ----

ENABLE_TESTING()

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(ElectionGuardTests)

if (USE_DYNAMIC_ANALYSIS AND VALGRIND_PROGRAM)
        message("++ Building with dynamic analysis")
        USE_VALGRIND(ElectionGuardTests --trace-children=yes --leak-check=full --sim-hints=no-nptl-pthread-stackcache --track-fds=yes)
endif()

if (CODE_COVERAGE)
    target_code_coverage(ElectionGuardTests AUTO)
endif()
