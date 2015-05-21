cmake_minimum_required(VERSION 2.8)
project(Parser)

set(Parser_VERISON_MAJOR 1)
set(Parser_VERSION_MINOR 0)

set(Parser_SOURCES
    AnotherDangParser.cpp
    OptionFlag.cpp)

set(ParserTest_SOURCES
    main.cpp)

add_library(Parser
    ${Parser_SOURCES})

add_executable(ParserTest
    ${ParserTest_SOURCES})

target_link_libraries(ParserTest Parser)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()


