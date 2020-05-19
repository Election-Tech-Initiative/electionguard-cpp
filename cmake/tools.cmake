# ---- Options ----

option(CODE_COVERAGE            "Enable code coverage"  OFF)
option(USE_STATIC_ANALYSIS      "use static analysis tools" OFF)
option(USE_DYNAMIC_ANALYSIS      "use dynamic analysis tools"   ON)
option(USE_FORMATTING           "use formatting tools"  ON)

function(use_valgrind TARGET_NAME)
    
    set(VALGRIND_LOG ${PROJECT_BINARY_DIR}/valgrind.log)
    set(VALGRIND_SUPPRESS ${PROJECT_SOURCE_DIR}/valgrind-suppress.supp)

    # --log-file=${VALGRIND_LOG} 
    set(VALGRIND_CMD ${VALGRIND_PROGRAM} --error-exitcode=1 --suppressions=${VALGRIND_SUPPRESS} ${ARGN})
    add_custom_target(memcheck-${TARGET_NAME}
        COMMAND
            ${VALGRIND_CMD} ./${TARGET_NAME}
    )

endfunction()

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

if (USE_DYNAMIC_ANALYSIS)
    message("++ Running with dynamic analysis")
    find_package(Valgrind)
    if (VALGRIND_PROGRAM)
        
    endif()
endif()

if (USE_FORMATTING)
    message("++ Running with formatting")
    include(${StableCoder-cmake-scripts_SOURCE_DIR}/formatting.cmake)
endif()