#include "../includes/string.h"

#include <stdlib.h>

/**
 * @brief   Verifies if the length of the given string matches
 *          the len parameter.
 *
 *          When the string contains a null terminator (like `He\0llo`),
 *          the length is calculated up to the first null terminator,
 *          so the length of `He\0llo` would be 2, not 5. If the len
 *          equals 5 the function will return false.
 *
 * @param   {const char*} str -   the string to be checked
 * @param   {size_t}      len -   the length to be checked against
 *
 * @return  {bool}  -   true if the string length matches the given length,
 *                      false otherwise
 */
static bool rss_input_matches_length(const char* str, size_t len)
{
    size_t str_len = 0;
    while (str[str_len] != RSS_NULL_TERMINATOR)
    {
        str_len++;
    }
    return str_len == len;
}

/**
 * @brief   Creates a new rss_create_result structure with the given parameters.
 *
 * @param   {rss_string*}   string  -   the rss_string structure to be wrapped in a rss_create_result
 * @param   {bool}          error   -   indicates if there was an error during the creation of the rss_string
 * @param   {int}           error_code  -   the error code associated with the error, if any
 *
 * @return  {rss_create_result*} -   a pointer to the created rss_create_result structure
 */
static rss_create_result* rss_result_create(rss_string* string, bool error, int error_code)
{
    rss_create_result* result = malloc(sizeof(rss_create_result));
    if (result == nullptr)
    {
        return nullptr;
    }
    result->string = string;
    result->error = error;
    result->error_code = error_code;
    return result;
}

rss_create_result* rss(const char* str, const size_t len, const size_t cap)
{
    rss_string* s = malloc(sizeof(rss_string));
    if (s == nullptr)
    {
        return rss_result_create(
            nullptr,
            true,
            RSS_STRING_ERROR_MEMORY_ALLOCATION_FAILED
        );
    }

    // Make sure the input string matches the provided length.
    //
    if (!rss_input_matches_length(str, len))
    {
        free(s);
        return rss_result_create(
            nullptr,
            true,
            RSS_STRING_ERROR_STR_LEN_MISMATCH
        );
    }

    // Len cannot be greater than cap, as this would mean
    // that the string would not fit in the allocated
    // memory.
    //
    if (len > cap)
    {
        free(s);
        return rss_result_create(
            nullptr,
            true,
            RSS_STRING_ERROR_LENGTH_GREATER_THAN_CAPACITY
        );
    }

    // Initialize the rss_string structure
    //
    s->array = malloc(sizeof(char) * (cap + 1));
    s->cap = cap;
    s->len = len;

    // Copy the string into the array
    //
    for (size_t i = 0; i < len; i++)
    {
        s->array[i] = str[i];
    }

    // Add the null terminator at the
    // end of the string
    //
    s->array[len] = '\0';

    return rss_result_create(
        s,
        false,
        RSS_STRING_ERROR_NONE
    );
}

bool rss_string_contains_char(const rss_string* s, char c)
{
    if (s == nullptr || s->array == nullptr)
    {
        return false;
    }

    for (size_t i = 0; i < s->len; i++)
    {
        if (s->array[i] == c)
        {
            return true;
        }
    }
    return false;
}

bool rss_string_contains_substring(const rss_string* rss, const char* substring)
{
    if (rss == nullptr || rss->array == nullptr || substring == nullptr)
    {
        return false;
    }

    size_t substring_len = 0;
    while (substring[substring_len] != RSS_NULL_TERMINATOR)
    {
        substring_len++;
    }

    if (substring_len == 0)
    {
        return true; // Empty substring is always found
    }

    for (size_t i = 0; i <= rss->len - substring_len; i++)
    {
        bool found = true;
        for (size_t j = 0; j < substring_len; j++)
        {
            if (rss->array[i + j] != substring[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return true;
        }
    }
    return false;
}

bool rss_string_length_between(const rss_string* s, size_t min_len, size_t max_len)
{
    if (s == nullptr)
    {
        return false;
    }
    return s->len >= min_len && s->len <= max_len;
}

bool rss_string_wipe(rss_string* s)
{
    if (s == nullptr || s->array == nullptr)
    {
        return false;
    }

    for (size_t i = 0; i < s->len; i++)
    {
        s->array[i] = '\0';
    }
    s->len = 0;
    return true;
}

