#include "../includes/alphabet.h"

#include <stdint.h>
#include <stdlib.h>

static rss_alphabet_create_result* rss_alphabet_create_result_create(rss_alphabet* alphabet, bool error, int error_code)
{
    rss_alphabet_create_result* result = malloc(sizeof(rss_alphabet_create_result));
    if (result == nullptr)
    {
        return nullptr;
    }

    result->alphabet = alphabet;
    result->error = error;
    result->error_code = error_code;

    return result;
}

rss_alphabet_create_result* rss_alphabet_create(const char* characters, size_t len)
{
    if (characters == nullptr)
    {
        return rss_alphabet_create_result_create(
            nullptr,
            true,
            RSS_ALPHABET_CREATION_ERROR_CHARACTERS_NULL
        );
    }

    if (len == SIZE_MAX)
    {
        return rss_alphabet_create_result_create(
            nullptr,
            true,
            RSS_ALPHABET_CREATION_ERROR_LEN_SIZE_MAX
        );
    }

    rss_alphabet* alphabet = malloc(sizeof(rss_alphabet));
    if (alphabet == nullptr)
    {
        return rss_alphabet_create_result_create(
            nullptr,
            true,
            RSS_ALPHABET_CREATION_ERROR_MEMORY_ALLOCATION_FAILED
        );
    }

    alphabet->array = malloc(sizeof(char) * (len + 1));
    if (alphabet->array == nullptr)
    {
        free(alphabet);
        return rss_alphabet_create_result_create(
            nullptr,
            true,
            RSS_ALPHABET_CREATION_ERROR_MEMORY_ALLOCATION_FAILED
        );
    }

    for (size_t i = 0; i < len; i++)
    {
        alphabet->array[i] = characters[i];
    }

    alphabet->array[len] = '\0';


    alphabet->len = len;


    return rss_alphabet_create_result_create(
        alphabet,
        false,
        RSS_ALPHABET_CREATION_ERROR_NONE
    );
}

void rss_alphabet_destroy(rss_alphabet* alphabet)
{
    if (alphabet == nullptr)
    {
        return;
    }

    free(alphabet->array);
    free(alphabet);
}

bool rss_alphabet_accepts(const rss_alphabet* alphabet, const rss_string* s)
{
    if (s == nullptr || s->array == nullptr || alphabet == nullptr)
    {
        return false;
    }

    for (size_t i = 0; i < s->len; i++)
    {
        bool found = false;
        for (size_t j = 0; j < alphabet->len; j++)
        {
            if (s->array[i] == alphabet->array[j])
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }
    return true;
}
