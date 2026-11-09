#include "../src/alphabet.c"

#include <assert.h>
#include <stdlib.h>

static void test_rss_alphabet_accepts()
{
    // Test case 1: string only contains characters from the alphabet
    //
    const char* str1 = "abc";
    size_t len1 = 3;
    size_t cap1 = 10;
    rss_create_result* result1 = rss(str1, len1, cap1);
    assert(result1 != nullptr);
    assert(result1->error == false);
    assert(result1->error_code == RSS_STRING_ERROR_NONE);
    assert(result1->string != nullptr);
    const rss_alphabet alphabet1 = { "abcde", 5 };
    assert(rss_alphabet_accepts(&alphabet1, result1->string) == true);
    free(result1->string->array);
    free(result1->string);
    free(result1);

    // Test case 2: string contains characters not in the alphabet
    //
    const char* str2 = "abcx";
    size_t len2 = 4;
    size_t cap2 = 10;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    const rss_alphabet alphabet2 = { "abcde", 5 };
    assert(rss_alphabet_accepts(&alphabet2, result2->string) == false);
    free(result2->string->array);
    free(result2->string);
    free(result2);

    // Test case 3: null terminator in the string
    //
    rss_string *string3 = malloc(sizeof(rss_string));
    size_t len3 = 6;
    size_t cap3 = 10;
    string3->array = malloc(sizeof(char) * (cap3 + 1));
    string3->array[0] = 'a';
    string3->array[1] = 'b';
    string3->array[2] = RSS_NULL_TERMINATOR; // Null terminator in the middle
    string3->array[3] = 'c';
    string3->array[4] = 'd';
    string3->array[5] = 'e';
    string3->array[len3] = RSS_NULL_TERMINATOR;

    string3->len = len3;
    string3->cap = cap3;

    const rss_alphabet alphabet3 = { "abcde", 5 };
    assert(rss_alphabet_accepts(&alphabet3, string3) == false);
}

int main()
{
    test_rss_alphabet_accepts();
    return 0;
}