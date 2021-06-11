#set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer ")
add_executable(trivial trivial.cpp)

add_executable(reader reader.cpp test_main.cpp)

add_executable(test2 test.cpp)

add_executable(token test_main.cpp Token.cpp)
target_link_libraries(token lox)
add_test(token token)

add_executable(value test_main.cpp value.cpp)
target_link_libraries(value lox)
add_test(value value)

add_executable(scanner test_main.cpp scanner.cpp)
target_link_libraries(scanner lox)
add_test(scanner scanner)

add_executable(parser test_main.cpp parser.cpp)
target_link_libraries(parser lox)
add_test(parser parser)
