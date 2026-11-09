#include "../includes/alphabet.h"

#include <stdlib.h>

rss_alphabet* rss_alphabet_create(const char* characters, size_t len)
{
    rss_alphabet *alphabet = malloc(sizeof(rss_alphabet));
    if (alphabet == nullptr)
    {
        return nullptr;
    }

    alphabet->array = malloc(sizeof(char) * (len + 1));
    if (alphabet->array == nullptr)
    {
        free(alphabet);
        return nullptr;
    }

    for (size_t i = 0; i < len; i++)
    {
        alphabet->array[i] = characters[i];
    }

    alphabet->array[len] = '\0';


    alphabet->len = len;


    return alphabet;
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