cmake_minimum_required(VERSION 3.9)

project(
    ddouble
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "library for emulated extended-precision arithmetic"
    )

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(ddouble SHARED
    src/ddouble.cxx
    )

# Require Catch2 for tests
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
    )
FetchContent_MakeAvailable(Catch2)

add_executable(tests
    test/arith.cxx)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain ddouble)
target_include_directories(tests PRIVATE src)
