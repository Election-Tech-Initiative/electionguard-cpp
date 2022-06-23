cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

set(CPP_BENCHMARK_TARGET      "ElectionGuardBenchmark")

set(UTILS_PROJECT_TARGET      "ElectionGuardUtils")
set(CPPTEST_PROJECT_TARGET    "ElectionGuardTests")
set(CTEST_PROJECT_TARGET      "ElectionGuardCTests")

# --- Import tools ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
    NAME doctest
    GITHUB_REPOSITORY doctest/doctest
    GIT_TAG v2.4.9
)

CPMAddPackage(
  NAME benchmark
  GITHUB_REPOSITORY google/benchmark
  VERSION 1.5.2
  OPTIONS "BENCHMARK_ENABLE_TESTING Off"
)

if(benchmark_ADDED)
  # enable c++11 to avoid compilation errors
  set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
endif()

# ---- Sources ----

# Utils Library -----------------------------------------------

add_library(${UTILS_PROJECT_TARGET} SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/ballot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/ballot.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/ballot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/election.h
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/election.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/election.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/manifest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/manifest.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/generators/manifest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/byte_logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/constants.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/utils/utils.h
)
add_library(${UTILS_PROJECT_TARGET}::${UTILS_PROJECT_TARGET} ALIAS ${UTILS_PROJECT_TARGET})

target_compile_features(${UTILS_PROJECT_TARGET} PRIVATE cxx_std_17)
target_compile_options(${UTILS_PROJECT_TARGET} PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")

# define dll export markers
if (BUILD_SHARED_LIBS)
    message("++ Building Shared Libs")  
    add_compile_definitions(ELECTIONGUARD_BUILD_SHARED)
endif()

if(MSVC)
    #target_compile_options(${UTILS_PROJECT_TARGET} /WX-)
    # TODO: validate the compiler warnings
    target_compile_options(${UTILS_PROJECT_TARGET} 
        PUBLIC /WX- /wd4067 /wd4100 /wd4266 /wd4365 /wd4464 /wd4505 /wd4554 /wd4668 /wd4820 /wd4996 /wd5045 /wd5059)
endif()


# C++ Benchmark -----------------------------------------------

add_executable(${CPP_BENCHMARK_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_chaum_pedersen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_elgamal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_encrypt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_hacl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_hashed_elgamal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_nonces.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/benchmark/bench_precompute.cpp
)

target_compile_features(${CPP_BENCHMARK_TARGET} PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${CPP_BENCHMARK_TARGET} PUBLIC -Wno-switch-default)
    target_compile_options(${CPP_BENCHMARK_TARGET} PUBLIC -Wno-unused-parameter)
    target_compile_options(${CPP_BENCHMARK_TARGET} PUBLIC -Wno-overloaded-virtual)
endif()


# C++ Tests ---------------------------------------------------

add_executable(${CPPTEST_PROJECT_TARGET} 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot_code.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot_compact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_chaum_pedersen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_constants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_election.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_elgamal.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt_compact.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hacl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_nonces.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_manifest.cpp
)

target_compile_features(${CPPTEST_PROJECT_TARGET} PRIVATE cxx_std_17)

if(MSVC)
    target_compile_definitions(${CPPTEST_PROJECT_TARGET} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)

    #target_compile_options(${UTILS_PROJECT_TARGET} /WX-)
    # TODO: validate the compiler warnings
    target_compile_options(${CPPTEST_PROJECT_TARGET} 
        PUBLIC /WX- /wd4067 /wd4189 /wd4251 /wd4266 /wd4365 /wd4464 /wd4505 /wd4554 /wd4623 /wd4625 /wd4626 /wd4668 /wd4820 /wd4866 /wd4996 /wd5027 /wd5045 /wd5059 /wd5219)
endif()

# C Tests -----------------------------------------------------

add_executable(${CTEST_PROJECT_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot_code.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_ballot.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_chaum_pedersen.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_collections.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_election.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_elgamal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt_compact.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_encrypt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_group.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/electionguard/test_manifest.c
)

target_compile_features(${CTEST_PROJECT_TARGET} PRIVATE c_std_99)

# ---- Link Libraries ----

target_link_libraries(${UTILS_PROJECT_TARGET} PUBLIC ${META_PROJECT_TARGET})

target_link_libraries(${CPP_BENCHMARK_TARGET} PRIVATE benchmark::benchmark ${UTILS_PROJECT_TARGET} ${META_PROJECT_TARGET})
target_link_libraries(${CPPTEST_PROJECT_TARGET} PRIVATE doctest ${UTILS_PROJECT_TARGET} ${META_PROJECT_TARGET})

if (MINGW)
    # Statically link in libstdc++
    set(CMAKE_EXE_LINKER_FLAGS " -static")
    target_link_libraries(${CPPTEST_PROJECT_TARGET} PRIVATE -static-libstdc++)
endif()

target_link_libraries(${CTEST_PROJECT_TARGET} PRIVATE ${UTILS_PROJECT_TARGET} ${META_PROJECT_TARGET})

if (WIN32)
    # sometimes windows struggles with paths (particularly in CI)
    # so, copy the .dll file to the test execution folder
    add_custom_command(
        TARGET ${CPPTEST_PROJECT_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:${META_PROJECT_TARGET}>
        $<TARGET_FILE_DIR:${CPPTEST_PROJECT_TARGET}>)
endif()

# ---- Add Tests ----

ENABLE_TESTING()

if (WIN32)
    include(${doctest_SOURCE_DIR}/scripts/cmake/common.cmake)
    doctest_add_test(NAME ${CPPTEST_PROJECT_TARGET} COMMAND $<TARGET_FILE:${CPPTEST_PROJECT_TARGET}> --no-version)
else()
    include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
    doctest_discover_tests(${CPPTEST_PROJECT_TARGET})
endif()

if (USE_DYNAMIC_ANALYSIS AND VALGRIND_PROGRAM)
        message("++ Building with dynamic analysis")
        USE_VALGRIND(${CPPTEST_PROJECT_TARGET} --trace-children=yes --leak-check=full --sim-hints=no-nptl-pthread-stackcache --track-fds=yes)
endif()

if (CODE_COVERAGE)
    target_code_coverage(${CPPTEST_PROJECT_TARGET} AUTO)
endif()
