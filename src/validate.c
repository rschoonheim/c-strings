#include "../includes/validate.h"

static romano_schoonheim_c_strings_validation_result_t result_make(bool error, const char * error_message)
{
    romano_schoonheim_c_strings_validation_result_t result;
    result.error = error;
    result.error_message = error_message;
    return result;
}

romano_schoonheim_c_strings_validation_result_t romano_schoonheim_c_string_contains_null_terminator()
{
    return result_make(false, "Not implemented yet");
}