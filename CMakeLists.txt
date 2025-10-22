cmake_minimum_required(VERSION 3.15)
project(AlgebraPlayground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===============================
# Include directories
# ===============================
include_directories(include)
include_directories(cli)

# ===============================
# Source files
# ===============================
set(SRC_FILES
    src/Matrix.cpp
    src/Workspace.cpp
    cli/CLI.cpp
)

# ===============================
# CLI Executable
# ===============================
add_executable(prog
    main.cpp
    ${SRC_FILES}
)

# ===============================
# Matrix Unit Tests
# ===============================
add_executable(matrixTests
    tests/matrixTests.cpp
    src/Matrix.cpp
)

# ===============================
# Output directory (optional)
# ===============================
set_target_properties(prog matrixTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ===============================
# Enable warnings
# ===============================
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4)
endif()
