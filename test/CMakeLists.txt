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

# Utils Library
add_library(ElectionGuardUtils SHARED
${CMAKE_CURRENT_SOURCE_DIR}/electionguard/mocks/ballot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/mocks/ballot.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/mocks/ballot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/mocks/election.h
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/mocks/election.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/mocks/election.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/constants.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/utils.h
)

target_compile_features(ElectionGuardUtils PRIVATE cxx_std_17)

# C++ Tests
add_executable(ElectionGuardTests 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp    
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot_compact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_chaum_pedersen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_election.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_elgamal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt_compact.cpp
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
    
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_chaum_pedersen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_election.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_elgamal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt_compact.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_tracker.c
)

target_compile_features(ElectionGuardCTests PRIVATE c_std_99)

# ---- Link Libraries ----

target_link_libraries(ElectionGuardUtils PUBLIC ${META_PROJECT_TARGET})

if (WIN32)
    # Statically link in libstdc++
    set(CMAKE_EXE_LINKER_FLAGS " -static")
    target_link_libraries(ElectionGuardTests PRIVATE doctest ElectionGuardUtils ${META_PROJECT_TARGET} -static-libstdc++)
else()
    target_link_libraries(ElectionGuardTests PRIVATE doctest ElectionGuardUtils ${META_PROJECT_TARGET})
endif()

target_link_libraries(ElectionGuardCTests PRIVATE ElectionGuardUtils ${META_PROJECT_TARGET})

if (WIN32)
    # sometimes windows struggles with paths (particularly in CI)
    # so, copy the .dll file to the test execution folder
    add_custom_command(
        TARGET ElectionGuardTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:${META_PROJECT_TARGET}>
        $<TARGET_FILE_DIR:ElectionGuardTests>)
endif()

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
