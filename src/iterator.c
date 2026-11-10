#include "../includes/iterator.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

static rss_iterator_create_result* rss_iterator_create_result_create(
    rss_string_iterator* iterator, bool error, int error_code)
{
    rss_iterator_create_result* result = malloc(sizeof(rss_iterator_create_result));
    if (result == nullptr)
    {
        return nullptr;
    }

    result->iterator = iterator;
    result->error = error;
    result->error_code = error_code;

    return result;
}

rss_iterator_create_result* rss_iterator_create(rss_string* string)
{
    if (string == nullptr)
    {
        return rss_iterator_create_result_create(
            nullptr,
            true,
            RSS_ITERATOR_ERROR_STRING_NULL
        );
    }

    rss_string_iterator* iterator = malloc(sizeof(rss_string_iterator));
    if (iterator == nullptr)
    {
        return rss_iterator_create_result_create(
            nullptr,
            true,
            RSS_ITERATOR_ERROR_MEMORY_ALLOCATION_FAILED
        );
    }

    iterator->string = string;
    iterator->current_index = 0;

    return rss_iterator_create_result_create(
        iterator,
        false,
        RSS_ITERATOR_ERROR_NONE
    );
}

bool rss_iterator_destroy(rss_string_iterator* iterator, const bool secure)
{
    if (iterator == nullptr || iterator->string == nullptr)
    {
        return false;
    }

    // When secure is true, we want to overwrite
    // the contents of the underlying rss_string
    // with zeros before freeing it.
    //
    if (secure)
    {
        rss_wipe(iterator->string);
    }

    // Free the underlying rss_string and
    // its array
    //
    free(iterator->string->array);
    free(iterator->string);

    // Free the iterator itself
    //
    free(iterator);
    return true;
}

bool rss_iterator_at_end(const rss_string_iterator* iterator)
{
    if (iterator == nullptr || iterator->string == nullptr)
    {
        return true;
    }

    return iterator->current_index >= iterator->string->len;
}

bool rss_iterator_index_move(rss_string_iterator* iterator, size_t amount)
{
    if (iterator == nullptr || iterator->string == nullptr)
    {
        return false;
    }

    if (iterator->current_index + amount > iterator->string->len)
    {
        return false;
    }

    if (iterator->current_index + amount < 0)
    {
        return false;
    }

    iterator->current_index += amount;
    return true;
}

char rss_iterator_current_char(const rss_string_iterator* iterator)
{
    if (iterator == nullptr || iterator->string == nullptr)
    {
        return RSS_NULL_TERMINATOR;
    }

    if (iterator->current_index >= iterator->string->len)
    {
        return RSS_NULL_TERMINATOR;
    }

    return iterator->string->array[iterator->current_index];
}

rss_iterator_look_result rss_iterator_look(const rss_string_iterator* iterator, size_t amount)
{
    if (iterator == nullptr || iterator->string == nullptr)
    {
        return (rss_iterator_look_result){
            .direction = RSS_ITERATOR_LOOK_DIRECTION_NONE,
            .character = RSS_NULL_TERMINATOR,
            .error = true,
            .error_code = RSS_ITERATOR_ERROR_STRING_NULL
        };
    }

    // Make sure that the look index does not exceed the
    // length of the string.
    //
    size_t look_index = iterator->current_index + amount;
    if (look_index >= iterator->string->len)
    {
        return (rss_iterator_look_result){
            .direction = RSS_ITERATOR_LOOK_DIRECTION_NONE,
            .character = RSS_NULL_TERMINATOR,
            .error = true,
            .error_code = RSS_ITERATOR_ERROR_OUT_OF_BOUNDS
        };
    }

    // Determine the direction of the look based on the amount
    //
    int direction = (amount > 0) ? RSS_ITERATOR_LOOK_DIRECTION_FORWARD :
                    (amount < 0) ? RSS_ITERATOR_LOOK_DIRECTION_BACKWARD :
                    RSS_ITERATOR_LOOK_DIRECTION_NONE;

    return (rss_iterator_look_result){
    .direction = direction,
    .character = iterator->string->array[look_index],
    .error = false,
    .error_code = RSS_ITERATOR_ERROR_NONE
    };
}