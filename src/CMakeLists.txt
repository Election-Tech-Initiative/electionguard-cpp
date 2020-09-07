cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)


# ---- Library Sources ----

set(PROJECT_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/electionguard/ballot.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/ballot_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/chaum_pedersen.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/chaum_pedersen_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/election.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/election_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/elgamal.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/elgamal_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/encrypt.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/encrypt_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/group.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/group_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/hash.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/hash_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/log.hpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/nonces.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/nonces.hpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/tracker.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/tracker_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/variant_cast.hpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/words.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/words.hpp
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Bignum256.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Bignum256.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Bignum4096.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Bignum4096.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Blake2b_32.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Blake2b_32.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Blake2s_32.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Blake2s_32.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Hash.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Hash.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Impl_Blake2_Constants.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Spec.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Spec.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Streaming_SHA2.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Hacl_Streaming_SHA2.h
    ${PROJECT_SOURCE_DIR}/src/kremlin/Lib_Memzero.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Lib_Memzero0.c
    ${PROJECT_SOURCE_DIR}/src/kremlin/Lib_Memzero0.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/ballot.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/ballot.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/chaum_pedersen.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/chaum_pedersen.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/constants.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/crypto_hashable.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/election.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/election.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/election_object_base.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/elgamal.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/elgamal.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/encrypt.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/encrypt.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/export.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/group.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/group.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/tracker.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/hash.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/hash.h
)

add_library(${META_PROJECT_TARGET} ${PROJECT_SOURCE_FILES})
add_library(${META_PROJECT_TARGET}::${META_PROJECT_TARGET} ALIAS ${META_PROJECT_TARGET})

# ---- Compile ----

# Set the target compile features rather than setting CMAKE_CXX_STANDARD 
# so we don't pollute other targets that may compile differently
target_compile_features(${META_PROJECT_TARGET} PRIVATE cxx_std_17)

target_compile_options(${META_PROJECT_TARGET} PUBLIC "$<$<BOOL:${MSVC}>:/permissive->")

if (MINGW)
    # Remove lib prefix
    set_target_properties(${META_PROJECT_TARGET} PROPERTIES PREFIX "")
    # define dll export markers
    add_compile_definitions(ELECTIONGUARD_BUILD_SHARED)
endif()

# Set the public include directory depending on 
# if the target is being exported or installed
target_include_directories(${META_PROJECT_TARGET}
    SYSTEM PUBLIC
        $<INSTALL_INTERFACE:include/${META_PROJECT_EXPORT}/${PROJECT_VERSION}>
        # Headers used from /build
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/${META_PROJECT_TARGET}
        ${PROJECT_SOURCE_DIR}/src/kremlin
)

# ---- Dependencies ----

# find_package(GMP REQUIRED)
# target_link_libraries(${META_PROJECT_TARGET} PRIVATE ${GMP_LIBRARY})

# ---- Install ----

# Install the electionguard library in the default location, 
# and associate electionguard with the ElectionGuard export
install(
    TARGETS ${META_PROJECT_TARGET} 
    EXPORT ${META_PROJECT_EXPORT}
)

# Install public header files
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/${META_PROJECT_EXPORT}/${PROJECT_VERSION}
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h*"
)

# Generate the build-tree ElectionGuardConfig.cmake for use 
# in other cmake projects without needing to install
export(
    EXPORT ${META_PROJECT_EXPORT}
    FILE "${PROJECT_BINARY_DIR}/${META_PROJECT_EXPORT}Config.cmake"
)

# Generate the install-tree ElectionGuardConfig.cmake for use 
# in other cmake projects after this library has been installed
install(
    EXPORT ${META_PROJECT_EXPORT}
    FILE ${META_PROJECT_EXPORT}Config.cmake
    DESTINATION lib/cmake/${META_PROJECT_EXPORT}/${PROJECT_VERSION}
)

# ---- Test ----

if(OPTION_ENABLE_TESTS)
    message("++ Building for tests")

    # Expose the internal headers to tests
    target_include_directories(${META_PROJECT_TARGET}
        SYSTEM PUBLIC
            #$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/src>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    )

    # When running tests, we set some specific compiler flags
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${META_PROJECT_TARGET} PUBLIC -Werror -Wall -Wextra -pedantic -pedantic-errors -Wunreachable-code -Wmissing-field-initializers -Wno-unused-parameter -Wno-unused-variable -Wno-unknown-pragmas)
    elseif(MSVC)
        target_compile_options(${META_PROJECT_TARGET} PUBLIC /W4 /WX)
    endif()

endif()

if (CODE_COVERAGE)
    message("++ Building with coverage")
    target_code_coverage(${META_PROJECT_TARGET} AUTO)
endif()

if (USE_FORMATTING)
    message("++ Building with formatting")
    clang_format(format ${PROJECT_SOURCE_FILES})
endif()
