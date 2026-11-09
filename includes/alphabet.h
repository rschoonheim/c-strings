#ifndef ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H
#define ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H

#include "string.h"

typedef struct rss_alphabet
{
    char* array;
    size_t len;
} rss_alphabet;

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
 * @brief   does the rss_alphabet accept the given rss_string?
 *
 * @param   {const rss_string*} s - the rss_string to be checked
 * @param   {const rss_alphabet*} alphabet - the alphabet to be checked against
 *
 * @return  {bool} - true if the string only contains characters from the alphabet, false otherwise
 */
bool rss_alphabet_accepts(const rss_alphabet* alphabet, const rss_string* s);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H
