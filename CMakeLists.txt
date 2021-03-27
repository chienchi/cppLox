cmake_minimum_required(VERSION 3.17)
project(cppLox)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 QUIET)

enable_testing()

# Enable Address Sanitizer
# set(GCC_ASAN_FLAG -fsanitize=address )
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${GCC_ASAN_FLAG}")
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

include_directories(.)
add_subdirectory(tests/c++)

set(SOURCES
        Token.cpp
        value.cpp
        scanner.cpp
        Parser.cpp Parser.h)
add_library(lox ${SOURCES})

add_executable(cppLox CppLox.cpp)
target_link_libraries(cppLox lox)

