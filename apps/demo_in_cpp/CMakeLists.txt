cmake_minimum_required(VERSION 3.14...3.16 FATAL_ERROR)

# ---- Project ----

project("ElectionGuard Demo in CPP" 
    VERSION 0.0.1 
    LANGUAGES CXX
)

# --- Import tools ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")

# ---- Dependencies ----

# ---- Executable Sources ----

add_executable(DemoInCPP
    ${PROJECT_SOURCE_DIR}/main.cpp
)

find_package(ElectionGuard REQUIRED)

target_compile_features(DemoInCPP PRIVATE cxx_std_17)

# ---- Link Libraries ----

target_link_libraries(DemoInCPP electionguard)