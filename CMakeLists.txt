cmake_minimum_required(VERSION 4.3)
project(romano_schoonheim_c_strings C)

set(CMAKE_C_STANDARD 23)

enable_testing()

add_executable(
        main
        main.c
)

add_library(
        romano_schoonheim_c_strings
        STATIC
        includes/array.h
        src/array.c
        includes/string.h
        src/string.c
        includes/alphabet.h
        src/alphabet.c
        includes/iterator.h
        src/iterator.c
)

add_executable(
        01_test_array
        test/01-test-array.c
)
target_link_libraries(01_test_array romano_schoonheim_c_strings)
add_test(
        NAME 01_test_array
        COMMAND 01_test_array
)


add_executable(
        02_test_string
        test/02-test-string.c
)
target_link_libraries(02_test_string romano_schoonheim_c_strings)
add_test(
        NAME 02_test_string
        COMMAND 02_test_string
)

add_executable(
        03_test_alphabet
        test/03-test-alphabet.c
)
target_link_libraries(03_test_alphabet romano_schoonheim_c_strings)
add_test(
        NAME 03_test_alphabet
        COMMAND 03_test_alphabet
)

add_executable(
        04_test_iterator
        test/04-test-iterator.c
)
target_link_libraries(04_test_iterator romano_schoonheim_c_strings)
add_test(
        NAME 04_test_iterator
        COMMAND 04_test_iterator
)
