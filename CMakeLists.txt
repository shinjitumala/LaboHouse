cmake_minimum_required(VERSION 3.5)

# Project name
project(LaboHousePrototype)

# C++ Requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# Global flags
add_compile_options(-Wall # enable all warnings, because why not?
                    -g3 -fno-rtti -fno-exceptions)

# List all sources here.
add_executable(
  LaboHouse
  src/main.cpp
  src/debug/Log.cpp
  src/util/fdstreambuf.cpp
  src/server/http/Request.cpp
  src/server/http/Response.cpp
  src/server/http/Body.cpp
  src/LaboHouse.cpp
  src/house/User.cpp
  src/house/Users.cpp)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

target_include_directories(LaboHouse
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
  LaboHouse
  # LLVM
  pthread)
