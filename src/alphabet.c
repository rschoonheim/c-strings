#include "../includes/alphabet.h"

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