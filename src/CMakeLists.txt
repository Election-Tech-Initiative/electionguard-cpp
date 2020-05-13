cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# --- Properties ---

# Set a DEBUG definition
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("++ Setting DEBUG during compile")
    add_compile_definitions(DEBUG)
endif()

# ---- Library Sources ----

set(PROJECT_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/electionguard/encrypt_facade.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/encrypt.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/hash.cpp
    ${PROJECT_SOURCE_DIR}/src/electionguard/hash.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/encrypt.h
    ${PROJECT_SOURCE_DIR}/include/electionguard/encrypt.hpp
    ${PROJECT_SOURCE_DIR}/include/electionguard/export.h
)

# TODO: STATIC
add_library(${META_PROJECT_TARGET} SHARED ${PROJECT_SOURCE_FILES})

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
)

# ---- Dependencies ----

find_package(GMP REQUIRED)
target_link_libraries(${META_PROJECT_TARGET} ${GMP_LIBRARY})

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
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    )

    # When running tests, we set some specific compiler flags
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${META_PROJECT_TARGET} PUBLIC -Werror -Wall -Wextra -pedantic -pedantic-errors -Wunreachable-code -Wmissing-field-initializers -lgmp)
    elseif(MSVC)
        target_compile_options(${META_PROJECT_TARGET} PUBLIC /W4 /WX)
    endif()

endif()
