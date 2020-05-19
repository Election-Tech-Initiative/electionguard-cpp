find_path(VALGRIND_INCLUDE_DIR 
    memcheck.h
    /usr/include 
    /usr/include/valgrind 
    /usr/local/include 
    /usr/local/include/valgrind
)
find_program(VALGRIND_PROGRAM 
    NAMES valgrind 
    PATH /usr/bin /usr/local/bin
)

mark_as_advanced(VALGRIND_INCLUDE_DIR VALGRIND_PROGRAM)

# Write log output
file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
    "Determing location of valgrind:\n"
    "Include directory: ${VALGRIND_INCLUDE_DIR}\n"
    "Program directory: ${VALGRIND_PROGRAM}\n\n")
