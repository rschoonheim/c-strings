cmake_minimum_required(VERSION 4.3)
project(romano_schoonheim_c_strings C)

set(CMAKE_C_STANDARD 23)

enable_testing()

add_library(
        romano_schoonheim_c_strings
        STATIC
        includes/string.h
        src/string.c
)

add_executable(
        01_test_string
        test/01-test-string.c
)
target_link_libraries(
        01_test_string
        romano_schoonheim_c_strings
)

add_test(
        NAME 01_test_string
        COMMAND 01_test_string
)
