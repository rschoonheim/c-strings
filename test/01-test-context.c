#include "../includes/context.h"

#include "assert.h"

static void test_romano_schoonheim_c_strings_context_create()
{
    // Test scenario 1: nullptr as raw_input with len
    //
    romano_schoonheim_c_strings_context_t *context1 = romano_schoonheim_c_strings_context_create(nullptr, 0);
    assert(context1->raw_input == nullptr);
    assert(context1->raw_input_length == 0);
    assert(context1->input_error == nullptr);

    // Test scenario 2: valid raw_input and length
    //
    char *input = "Hello, World!";
    size_t length = 13;
    romano_schoonheim_c_strings_context_t *context2 = romano_schoonheim_c_strings_context_create(input, length);
    assert(context2->raw_input == input);
    assert(context2->raw_input_length == length);
    assert(context2->input_error == nullptr);
}

int main()
{
    test_romano_schoonheim_c_strings_context_create();
    return 0;
}