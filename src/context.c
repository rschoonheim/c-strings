#include "../includes/context.h"

#include <stdlib.h>

romano_schoonheim_c_strings_context_t * romano_schoonheim_c_strings_context_create(
    char * raw_input,
    size_t raw_input_length
    )
{
    romano_schoonheim_c_strings_context_t * context = malloc(sizeof(romano_schoonheim_c_strings_context_t));
    if (context == nullptr)
    {
        return nullptr;
    }

    context->raw_input = raw_input;
    context->raw_input_length = raw_input_length;

    return context;
}
