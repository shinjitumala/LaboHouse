cmake_minimum_required(VERSION 3.15)

# Project name
project(LaboHousePrototype)

# C++ Requirements
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# OS Check
if(NOT UNIX)
  message(
    FATAL_ERROR
      "Currently, LaboHouse is not implemented to work in non-UNIX OSes")
endif()

# Global flags
add_compile_options(-Wall # enable all warnings, because why not?
                    -g3 -fno-rtti)

# List all sources here.
add_executable(LaboHouse)
target_compile_features(LaboHouse PUBLIC cxx_std_17)
target_compile_options(LaboHouse PRIVATE -fsanitize=address)
target_include_directories(
  LaboHouse
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/json/single_include/
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/websocketpp)
target_link_libraries(
  LaboHouse
  # LLVM
  pthread)
target_link_options(LaboHouse PRIVATE -fsanitize=address)

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
          src/util/rand.cpp
          src/server/http/Request.cpp
          src/server/http/Response.cpp
          src/server/http/Html.cpp
          src/server/http/ResponseHelper.cpp
          src/server/Html.cpp
          src/server/WS.cpp
          src/LaboHouse.cpp
          src/house/User.cpp
          src/house/Users.cpp
          src/house/Chat.cpp
          src/house/Quote.cpp)
target_precompile_headers(LaboHouse PUBLIC include/labo/server/WS_Headers.h
                          extern/json/single_include/nlohmann/json.hpp)

# Configure HTML
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/res/js/script.js JS)
string(PREPEND JS "<script type=\"text/javascript\">")
string(APPEND JS "</script>")
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/res/js/labohouse_api.js API_JS)
string(PREPEND API_JS "<script type=\"text/javascript\">")
string(APPEND API_JS "</script>")
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/res/css/style.css CSS)
string(PREPEND CSS "<style>")
string(APPEND CSS "</style>")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/home.html
               ${CMAKE_CURRENT_BINARY_DIR}/res/home.html)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/res/not_found.html
               ${CMAKE_CURRENT_BINARY_DIR}/res/not_found.html)
set_property(
  DIRECTORY
  APPEND
  PROPERTY CMAKE_CONFIGURE_DEPENDS
           ${CMAKE_CURRENT_SOURCE_DIR}/res/css/style.css)
set_property(
  DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
                            ${CMAKE_CURRENT_SOURCE_DIR}/res/js/script.js)
set_property(
  DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
                            ${CMAKE_CURRENT_SOURCE_DIR}/res/js/labohouse_api.js)

# Start server
add_custom_target(start COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/start_server.sh)
