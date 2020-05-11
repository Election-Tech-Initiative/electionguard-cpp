# Find the headers
find_path(GMP_INCLUDES
    NAMES gmp.h gmpxx.h
)

# TODO: check GMP Version

# Verify the C++ header is avaialable
include(CMakePushCheckState)
cmake_push_check_state()
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${GMP_INCLUDES})
include(CheckIncludeFileCXX)
check_include_file_cxx("gmpxx.h" GMPXX_HEADER_AVAILABLE)

# Find the C Lib
find_library(GMP_LIBRARY NAMES gmp libgmp)

# Find the C++ Lib
find_library(GMPXX_LIBRARY NAMES gmpxx libgmpxx)

# Configure CMake Module
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP 
    DEFAULT_MSG
    GMP_INCLUDES
    GMP_LIBRARY
    GMPXX_LIBRARY
    GMPXX_HEADER_AVAILABLE
)
mark_as_advanced(
    GMP_INCLUDES 
    GMP_LIBRARY
    GMPXX_LIBRARY
)

# Write log output
file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
    "Determing location of GMP:\n"
    "Include directory: ${GMP_INCLUDES}\n"
    "GMP Lib directory: ${GMP_LIBRARY}\n"
    "GMP CXX directory: ${GMPXX_LIBRARY}\n"
    "GMP CXX hdr works: ${GMPXX_HEADER_AVAILABLE}\n\n")
