cmake_minimum_required(VERSION 3.10)
project(OmaniDB C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler warnings
add_compile_options(-Wall -Wextra -pedantic)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    main.c
    src/repl.c
    src/compiler.c
    src/vm.c
    src/storage.c
)

# Build the executable
add_executable(omanidb ${SOURCES})
