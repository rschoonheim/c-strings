#ifndef ROMANO_SCHOONHEIM_C_STRINGS_STRING_H
#define ROMANO_SCHOONHEIM_C_STRINGS_STRING_H
#include <stddef.h>

#define RSS_STRING_ERROR_NONE 0
#define RSS_STRING_ERROR_MEMORY_ALLOCATION_FAILED 1
#define RSS_STRING_ERROR_STR_LEN_MISMATCH 2
#define RSS_STRING_ERROR_LENGTH_GREATER_THAN_CAPACITY 3

typedef struct rss_string
{
    /**
     * @brief   array holding the characters of the string.
     *          the array has the size of cap + 1, to hold
     *          the null terminator.
     */
    char* array;
    /**
     * @brief   length holds the number of elements in the array
     *          currently used by the string.
     */
    size_t len;
    /**
     * @brief   capacity holds the total number of elements that
     *          the array can hold, not including the null
     *          terminator.
     */
    size_t cap;
} rss_string;

typedef struct rss_alphabet
{
    char* array;
    size_t len;
} rss_alphabet;

typedef struct rss_create_result
{
    bool error;
    int error_code;
    rss_string* string;
} rss_create_result;

/**
 * @brief   creates a new rss_string structure from a given string
 *
 * @param   {const char *} str - the string to be converted to a rss_string
 * @param   {size_t} len - the length of the string
 * @param   {size_t} cap - the capacity of the string structure
 *
 * @return  {rss_create_result*} - a pointer to a rss_create_result structure
 */
rss_create_result* rss(const char* str, size_t len, size_t cap);

/**
 * @brief   creates a new rss_alphabet structure from a given string
 *
 * @param   {const char *} characters - the string to be converted to a rss_alphabet
 * @param   {size_t} len - the length of the string
 *
 * @return  {rss_alphabet*} - a pointer to a rss_alphabet structure
 */
rss_alphabet* rss_alphabet_create(const char* characters, size_t len);

/**
 * @brief   does the rss_string contains the given character?
 *
 * @param   {const rss_string*} s - the rss_string to be searched
 * @param   {char} c - the character to be searched for
 *
 * @return {bool} - true if the character is found, false otherwise
 */
bool rss_string_contains_char(const rss_string* s, char c);

/**
 * @brief   does the rss_string contains the given substring?
 *
 * @param   {const rss_string*} rss - the rss_string to be searched
 * @param   {const char*} substring - the substring to be searched for
 *
 * @return  {bool} - true if the substring is found, false otherwise
 */
bool rss_string_contains_substring(const rss_string* rss, const char* substring);

/**
 * @brief   does the rss_alphabet accept the given rss_string?
 *
 * @param   {const rss_string*} s - the rss_string to be checked
 * @param   {const rss_alphabet*} alphabet - the alphabet to be checked against
 *
 * @return  {bool} - true if the string only contains characters from the alphabet, false otherwise
 */
bool rss_alphabet_accepts(const rss_alphabet* alphabet, const rss_string* s);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_STRING_H
