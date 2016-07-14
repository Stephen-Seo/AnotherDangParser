cmake_minimum_required(VERSION 2.8)
project(AnotherDangParser)

set(AnotherDangParser_VERSION_MAJOR 2)
set(AnotherDangParser_VERSION_MINOR 0)

set(AnotherDangParser_SOURCES
    src/ADP/AnotherDangParser.cpp
    src/ADP/OptionFlag.cpp
    src/ADP/HelpInfo.cpp
)

set(AnotherDangParser_HEADERS
    src/ADP/AnotherDangParser.hpp
    src/ADP/OptionFlag.hpp
    src/ADP/HelpInfo.hpp
)

set(ParserTest_SOURCES
    src/main.cpp)

add_library(AnotherDangParser
    ${AnotherDangParser_SOURCES})

add_executable(ParserTest
    ${ParserTest_SOURCES})

set_target_properties(AnotherDangParser PROPERTIES
    VERSION "${AnotherDangParser_VERSION_MAJOR}.${AnotherDangParser_VERSION_MINOR}"
)

target_link_libraries(ParserTest AnotherDangParser)

target_include_directories(ParserTest PUBLIC ${CMAKE_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

install(TARGETS AnotherDangParser
    LIBRARY DESTINATION usr/lib
)

install(FILES ${AnotherDangParser_HEADERS} DESTINATION usr/include/ADP)

