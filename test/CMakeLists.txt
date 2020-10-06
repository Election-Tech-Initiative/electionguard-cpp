cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

project("ElectionGuardTests"
    VERSION 0.0.1 
    LANGUAGES C CXX
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

# C++ Tests
add_executable(ElectionGuardTests 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/constants.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_chaum_pedersen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_elgamal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hacl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_nonces.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_tracker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_words.cpp
)

target_compile_features(ElectionGuardTests PRIVATE cxx_std_17)

if(MSVC)
    target_compile_definitions(ElectionGuardTests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
endif()

# C Tests
add_executable(ElectionGuardCTests
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_chaum_pedersen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_elgamal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_tracker.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/utils.h
)

target_compile_features(ElectionGuardCTests PRIVATE c_std_99)

# ---- Link Libraries ----

target_link_libraries(ElectionGuardTests PRIVATE doctest ${META_PROJECT_TARGET})
target_link_libraries(ElectionGuardCTests PRIVATE ${META_PROJECT_TARGET})

# ---- Add Tests ----

ENABLE_TESTING()

if (WIN32)
    include(${doctest_SOURCE_DIR}/scripts/cmake/common.cmake)
    doctest_add_test(NAME ElectionGuardTests COMMAND $<TARGET_FILE:ElectionGuardTests> --no-version)
else()
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
    doctest_discover_tests(ElectionGuardTests)
endif()

if (USE_DYNAMIC_ANALYSIS AND VALGRIND_PROGRAM)
        message("++ Building with dynamic analysis")
        USE_VALGRIND(ElectionGuardTests --trace-children=yes --leak-check=full --sim-hints=no-nptl-pthread-stackcache --track-fds=yes)
endif()

if (CODE_COVERAGE)
    target_code_coverage(ElectionGuardTests AUTO)
endif()
