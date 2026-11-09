#ifndef ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H
#define ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H

#include "string.h"

#define RSS_ALPHABET_CREATION_ERROR_NONE 0
#define RSS_ALPHABET_CREATION_ERROR_CHARACTERS_NULL 1
#define RSS_ALPHABET_CREATION_ERROR_LEN_SIZE_MAX 2
#define RSS_ALPHABET_CREATION_ERROR_MEMORY_ALLOCATION_FAILED 3

typedef struct rss_alphabet
{
    char* array;
    size_t len;
} rss_alphabet;

typedef struct rss_alphabet_create_result
{
    bool error;
    int error_code;
    rss_alphabet* alphabet;
} rss_alphabet_create_result;

/**
 * @brief   creates a new rss_alphabet structure from a given string
 *
 * @param   {const char *} characters - the string to be converted to a rss_alphabet
 * @param   {size_t} len - the length of the string
 *
 * @return  {rss_alphabet_create_result*} - a pointer to the result structure containing
 *                                          the created rss_alphabet or an error code
 */
rss_alphabet_create_result * rss_alphabet_create(const char* characters, size_t len);

/*
 * @brief   destroy the given rss_alphabet structure and free its memory
 *
 * @param   {rss_alphabet*} alphabet - the rss_alphabet to be destroyed
 *
 * @return  {void}
 */
void rss_alphabet_destroy(rss_alphabet* alphabet);

/**
 * @brief   does the rss_alphabet accept the given rss_string?
 *
 * @param   {const rss_string*} s - the rss_string to be checked
 * @param   {const rss_alphabet*} alphabet - the alphabet to be checked against
 *
 * @return  {bool} - true if the string only contains characters from the alphabet, false otherwise
 */
bool rss_alphabet_accepts(const rss_alphabet* alphabet, const rss_string* s);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H
