#include "../includes/iterator.h"

#include <stdlib.h>

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
            RSS_STRING_ITERATOR_ERROR_STRING_NULL
        );
    }

    rss_string_iterator* iterator = malloc(sizeof(rss_string_iterator));
    if (iterator == nullptr)
    {
        return rss_iterator_create_result_create(
            nullptr,
            true,
            RSS_STRING_ITERATOR_ERROR_MEMORY_ALLOCATION_FAILED
        );
    }

    iterator->string = string;
    iterator->current_index = 0;

    return rss_iterator_create_result_create(
        iterator,
        false,
        RSS_STRING_ITERATOR_ERROR_NONE
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