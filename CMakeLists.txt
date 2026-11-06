cmake_minimum_required(VERSION 4.3)
project(romano_schoonheim_c_strings C)

set(CMAKE_C_STANDARD 23)

enable_testing()

add_library(
        romano_schoonheim_c_strings
        STATIC
        includes/context.h
        src/context.c
        includes/validate.h
        src/validate.c
)

add_executable(
        01_test_context
        test/01-test-context.c
)
target_link_libraries(01_test_context romano_schoonheim_c_strings)

add_test(
        NAME 01_test_context
        COMMAND 01_test_context
)