#include "../src/alphabet.c"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void test_rss_alphabet_create()
{
    // Test case 1: successful creation
    //
    const char* characters1 = "abcde";
    size_t len1 = 5;
    rss_alphabet* alphabet1 = rss_alphabet_create(characters1, len1);
    assert(alphabet1 != nullptr);
    assert(alphabet1->len == len1);
    assert(strcmp(alphabet1->array, characters1) == 0);
    free(alphabet1->array);
    free(alphabet1);

    // Test case 2: empty string
    //
    const char* characters2 = "";
    size_t len2 = 0;
    rss_alphabet* alphabet2 = rss_alphabet_create(characters2, len2);
    assert(alphabet2 != nullptr);
    assert(alphabet2->len == len2);
    assert(strcmp(alphabet2->array, characters2) == 0);
    free(alphabet2->array);
    free(alphabet2);

    // Test case 3: null terminator can be part
    //              of the alphabet
    //
    const char* characters3 = "ab\0cd";
    size_t len3 = 5;
    rss_alphabet* alphabet3 = rss_alphabet_create(characters3, len3);
    assert(alphabet3 != nullptr);
    assert(alphabet3->len == len3);
    assert(alphabet3->array[0] == 'a');
    assert(alphabet3->array[1] == 'b');
    assert(alphabet3->array[2] == RSS_NULL_TERMINATOR);
    assert(alphabet3->array[3] == 'c');
    assert(alphabet3->array[4] == 'd');
    free(alphabet3->array);
    free(alphabet3);

    // Test case 4: characters is nullptr
    //
    const char* characters4 = nullptr;
    size_t len4 = 5;
    rss_alphabet* alphabet4 = rss_alphabet_create(characters4, len4);
    assert(alphabet4 == nullptr);

    // Test case 5: len is SIZE_MAX
    //
    const char* characters5 = "abcde";
    size_t len5 = SIZE_MAX;
    rss_alphabet* alphabet5 = rss_alphabet_create(characters5, len5);
    assert(alphabet5 == nullptr);
}


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
    test_rss_alphabet_create();
    test_rss_alphabet_accepts();
    return 0;
}