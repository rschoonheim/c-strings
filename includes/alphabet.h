#ifndef ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H
#define ROMANO_SCHOONHEIM_C_STRINGS_ALPHABET_H

#include "string.h"

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
