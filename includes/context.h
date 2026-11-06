#ifndef ROMANO_SCHOONHEIM_C_STRINGS_CONTEXT_H
#define ROMANO_SCHOONHEIM_C_STRINGS_CONTEXT_H
#include <stddef.h>

/**
 * @brief   Structure representing an input error encountered
 *          while processing a C string input.
 */
typedef struct romano_schoonheim_c_strings_input_error_t
{
    int error_code;
    const char * error_message;
} romano_schoonheim_c_strings_input_error_t;

/**
 * @brief   Structure representing the context for processing
 *          a C string input.
 */
typedef struct romano_schoonheim_c_strings_context_t
{
    char * raw_input;
    size_t raw_input_length;
    romano_schoonheim_c_strings_input_error_t * input_error;
} romano_schoonheim_c_strings_context_t;

/**
 * @brief   Creates a new context structure for processing C strings.
 *
 * @param   {char *}  raw_input             -   The raw input string to be processed.
 * @param   {size_t}  raw_input_length      -   The length of the raw input string.
 *
 * @return  {romano_schoonheim_c_strings_context_t *}   -   A pointer to the newly created context structure.
 */
romano_schoonheim_c_strings_context_t * romano_schoonheim_c_strings_context_create(char * raw_input, size_t raw_input_length);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_CONTEXT_H
