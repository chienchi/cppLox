#set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer ")
Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY  https://github.com/catchorg/Catch2.git
        GIT_TAG v2.13.1
)
FetchContent_MakeAvailable(Catch2)

add_library(test_main OBJECT test_main.cpp)

add_executable(trivial trivial.cpp)

add_executable(reader reader.cpp)
target_link_libraries(reader test_main Catch2::Catch2)
add_test(reader reader)

add_executable(test2 test.cpp)

add_executable(token Token.cpp)
target_link_libraries(token lox test_main Catch2::Catch2)
add_test(token token)

add_executable(value value.cpp)
target_link_libraries(value lox test_main Catch2::Catch2)
add_test(value value)

add_executable(scanner scanner.cpp)
target_link_libraries(scanner lox test_main Catch2::Catch2)
add_test(scanner scanner)

add_executable(parser parser.cpp)
target_link_libraries(parser lox test_main Catch2::Catch2)
add_test(parser parser)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)
catch_discover_tests(reader token value scanner parser)