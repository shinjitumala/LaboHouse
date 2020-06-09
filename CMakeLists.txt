cmake_minimum_required(VERSION 3.5)

# Project name
project(LaboHousePrototype)

# C++ Requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Global flags
add_compile_options(
    -Wall # enable all warnings, because why not?
    -g3
)

# List all sources here.
add_executable(LaboHouse
    src/main.cpp
    src/debug/Log.cpp
    src/server/http.cpp
    src/server/socket_stream.cpp
)

# Setup LLVM
# find_package(LLVM REQUIRED CONFIG)
# set(CMAKE_C_COMPILER ${LLVM_TOOLS_BINARY_DIR}/clang)
# set(CMAKE_CXX_COMPILER ${LLVM_TOOLS_BINARY_DIR}/clang++)
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)

target_include_directories(LaboHouse
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(LaboHouse
    # LLVM
    pthread
)