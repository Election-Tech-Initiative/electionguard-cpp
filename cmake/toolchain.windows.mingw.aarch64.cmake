# Toolchain file for cross-compiling for windows x86_64

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross compilers to use
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)

# Set the location of the target environment
# TODO: use relative path
set(CMAKE_FIND_ROOT_PATH /usr/local/Cellar/mingw-w64/8.0.0_3/toolchain-i686/i686-w64-mingw32)


# only search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Only search for programs in the build host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# HACK: TODO: disabling processor intrinsics for cross compilation for now
add_compile_definitions(BROKEN_INTRINSICS)
