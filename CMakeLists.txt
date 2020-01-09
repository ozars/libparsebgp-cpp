cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(libparsebgp-cpp)
read_version_from_file(LIBPARSEBGP_CPP_VERSION "${CMAKE_CURRENT_LIST_DIR}/VERSION")

project(libparsebgp-cpp VERSION "${LIBPARSEBGP_CPP_VERSION}")

find_package(ZLIB REQUIRED)

add_subdirectory(external)

add_library(parsebgp_cpp)
file(GLOB_RECURSE SOURCES src/*.cpp)
target_sources(parsebgp_cpp PRIVATE ${SOURCES})
target_include_directories(parsebgp_cpp
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(parsebgp_cpp PUBLIC parsebgp ZLIB::ZLIB span-lite
    optional-lite string-view-lite)
set_target_properties(parsebgp_cpp PROPERTIES CXX_STANDARD 17)
