cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

project("ElectionGuard Demo in C" 
    VERSION 0.0.1 
    LANGUAGES C
)

# --- Import tools ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

# ---- Dependencies ----

# ---- Executable Sources ----

add_executable(DemoInC
    ${PROJECT_SOURCE_DIR}/main.c
)

# ---- Link Libraries ----

find_package(ElectionGuard REQUIRED)

target_link_libraries(DemoInC electionguard)
