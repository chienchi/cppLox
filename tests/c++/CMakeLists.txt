#set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer ")
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
