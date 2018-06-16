cmake_minimum_required(VERSION 3.0)
project(AnotherDangParser VERSION 3.1)

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
    src/test.cpp)

add_library(AnotherDangParser
    ${AnotherDangParser_SOURCES})

add_executable(ParserTest
    ${ParserTest_SOURCES})

set_target_properties(AnotherDangParser PROPERTIES
    VERSION ${AnotherDangParser_VERSION}
    SOVERSION ${AnotherDangParser_VERSION}
)

target_link_libraries(ParserTest AnotherDangParser)

target_include_directories(ParserTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

target_compile_features(AnotherDangParser PUBLIC cxx_std_11)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

install(TARGETS AnotherDangParser
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${AnotherDangParser_HEADERS} DESTINATION include/ADP)
