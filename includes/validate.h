#ifndef ROMANO_SCHOONHEIM_C_STRINGS_VALIDATE_H
#define ROMANO_SCHOONHEIM_C_STRINGS_VALIDATE_H

#include "context.h"

typedef struct romano_schoonheim_c_strings_validation_result_t
{
    bool error;
    const char * error_message;
} romano_schoonheim_c_strings_validation_result_t;

/**
 * @brief   Checks if the string contains null terminator
 *          before the end of the specified length.
 *
 * @param   {romano_schoonheim_c_strings_context_t *} context - The context containing the raw input string and its length.
 *
 * @return  {romano_schoonheim_c_strings_validation_result_t} - The result of the validation, indicating if an error occurred and the corresponding error message.
 */
romano_schoonheim_c_strings_validation_result_t romano_schoonheim_c_string_contains_null_terminator();


#endif //ROMANO_SCHOONHEIM_C_STRINGS_VALIDATE_H
