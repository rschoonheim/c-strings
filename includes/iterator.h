#ifndef ROMANO_SCHOONHEIM_C_STRINGS_ITERATOR_H
#define ROMANO_SCHOONHEIM_C_STRINGS_ITERATOR_H

#include "string.h"

#define RSS_ITERATOR_ERROR_NONE 0
#define RSS_ITERATOR_ERROR_STRING_NULL 1
#define RSS_ITERATOR_ERROR_MEMORY_ALLOCATION_FAILED 2
#define RSS_ITERATOR_ERROR_EXCEEDING_MAX_INPUT 3
#define RSS_ITERATOR_ERROR_OUT_OF_BOUNDS 4

typedef struct rss_string_iterator
{
    rss_string* string;
    size_t current_index;
} rss_string_iterator;

typedef struct rss_string_iterator_create_result
{
    bool error;
    int error_code;
    rss_string_iterator* iterator;
} rss_iterator_create_result;

typedef struct rss_iterator_look_result
{
    enum
    {
        RSS_ITERATOR_LOOK_DIRECTION_FORWARD = 1,
        RSS_ITERATOR_LOOK_DIRECTION_NONE = 0,
        RSS_ITERATOR_LOOK_DIRECTION_BACKWARD = -1
    } direction;
    char character;
    bool error;
    int error_code;
} rss_iterator_look_result;

/**
 * @brief   Creates a new rss_string_iterator structure for the given rss_string
 *
 * @param   {rss_string*}   string - the rss_string to create an iterator for
 *
 * @return  {rss_string_iterator_create_result*} - a pointer to the result structure containing
 */
rss_iterator_create_result * rss_iterator_create(rss_string *string);

/**
 * @brief   Destroys the given rss_string_iterator structure and frees its memory
 *
 * @param   {rss_string_iterator*} iterator - the rss_string_iterator to be destroyed
 * @param   {bool} secure - if true, the underlying rss_string will also be wiped and freed
 * @param   {bool} destroy_string - if true, the underlying rss_string will also be freed
 *
 * @return  {bool} - true if the iterator was successfully destroyed, false otherwise
 */
bool rss_iterator_destroy(rss_string_iterator* iterator, bool secure, bool destroy_string);

/**
 * @brief   Are we at the end of the string?
 *
 * @param   {const rss_string_iterator*} iterator - the iterator to check
 *
 * @return  {bool} - true if the iterator is at the end of the string, false otherwise
 */
bool rss_iterator_at_end(const rss_string_iterator* iterator);

/**
 * @brief   Forward the current index of the iterator by given amount
 *
 * @param   {rss_string_iterator*} iterator - the iterator to forward
 * @param   {int} amount - the amount to forward the iterator by
 *
 * @return  {bool} - true if the iterator was successfully forwarded, false otherwise
 */
bool rss_iterator_index_move(rss_string_iterator* iterator, int amount);

/**
 * @brief   Get the current character of the iterator
 *
 * @param   {const rss_string_iterator*} iterator - the iterator to get the current character from
 *
 * @return  {char} - the current character of the iterator, or '\0' if the iterator is at the end of the string
 */
char rss_iterator_current_char(const rss_string_iterator* iterator);

/**
 * @brief   Look ahead or behind the current index of the iterator by given amount.
 *          When the amount is positive, we look ahead, when the amount is negative,
 *          we look behind.
 *
 * @param   {const rss_string_iterator*} iterator - the iterator to look ahead in
 * @param   {int} amount - the amount to look ahead or behind the iterator by.
 *
 *
 * @return  {rss_iterator_look_result} -    a structure containing the character found,
 *                                          the direction of the look,
 */
rss_iterator_look_result rss_iterator_look(const rss_string_iterator* iterator, int amount);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_ITERATOR_H
