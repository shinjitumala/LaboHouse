cmake_minimum_required(VERSION 3.15)

# Project name
project(LaboHousePrototype)

# C++ Requirements
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
find_program(HAS_CLANG clang)
if(HAS_CLANG)
  set(CMAKE_C_COMPILER clang)
endif()
find_program(HAS_CLANG clang++)
if(HAS_CLANG)
  set(CMAKE_CXX_COMPILER clang++)
endif()

# OS Check
if(NOT UNIX)
  message(
    FATAL_ERROR
      "Currently, LaboHouse is not implemented to work in non-UNIX OSes")
endif()

# Global flags
add_compile_options(-Wall # enable all warnings, because why not?
                    -g3 -fno-rtti -fno-exceptions)

# List all sources here.
add_executable(LaboHouse)
target_compile_features(LaboHouse PUBLIC cxx_std_17)
target_include_directories(LaboHouse
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(
  LaboHouse
  # LLVM
  pthread)

# documentation related
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/doxygen
  DEPENDS LaboHouse ${CMAKE_CURRENT_SOURCE_DIR}/Protocols.md
  COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_custom_target(gendoc DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/doxygen)
if(NOT DEFINED BROWSER)
  message(STATUS "No browser specified, using firefox.")
  set(BROWSER firefox)
endif()
find_program(HAS_BROWSER ${BROWSER})
if(NOT HAS_BROWSER)
  message(
    FATAL_ERROR
      "Could not find: ${BROWSER}.\n"
      "Please specify your browser with 'cmake .. -DBROWSER=<Your browser>'")
endif()
add_custom_target(
  doc
  COMMAND ${BROWSER} ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/index.html
  DEPENDS gendoc)
list(APPEND ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/doxygen)

# List of sources
target_sources(
  LaboHouse
  PRIVATE src/main.cpp
          src/debug/Log.cpp
          src/util/fdstreambuf.cpp
          src/server/http/Request.cpp
          src/server/http/Response.cpp
          src/server/http/Html.cpp
          src/server/http/ResponseHelper.cpp
          src/LaboHouse.cpp
          src/house/User.cpp
          src/house/Users.cpp)
