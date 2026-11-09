#include "../src/string.c"
#include "../src/alphabet.c"

#include <assert.h>
#include <string.h>

static void test_rss_input_matches_length()
{
    // Test case 1: matching length
    //
    const char* str1 = "Hello";
    size_t len1 = 5;
    assert(rss_input_matches_length(str1, len1) == true);

    // Test case 2: non-matching length
    //
    const char* str2 = "Hello";
    size_t len2 = 4;
    assert(rss_input_matches_length(str2, len2) == false);

    // Test case 3: empty string
    //
    const char* str3 = "";
    size_t len3 = 0;
    assert(rss_input_matches_length(str3, len3) == true);

    // Test case 4: early null terminator
    //
    const char* str4 = "He\0llo";
    size_t len4 = 5;
    assert(rss_input_matches_length(str4, len4) == false);
}

static void test_rss_result_create()
{
    // Test case 1: non error case
    //
    rss_create_result* string1 = rss_result_create(nullptr, false, RSS_STRING_ERROR_NONE);
    assert(string1 != nullptr);
    assert(string1->error == false);
    assert(string1->error_code == RSS_STRING_ERROR_NONE);
    free(string1);

    // Test case 2: error case
    //
    rss_create_result* string2 = rss_result_create(nullptr, true, RSS_STRING_ERROR_MEMORY_ALLOCATION_FAILED);
    assert(string2 != nullptr);
    assert(string2->error == true);
    assert(string2->error_code == RSS_STRING_ERROR_MEMORY_ALLOCATION_FAILED);
    free(string2);
}

static void test_rss()
{
    // Test case 1: mismatching length
    //
    const char* str1 = "Hello";
    size_t len1 = 4;
    size_t cap1 = 10;
    rss_create_result* result1 = rss(str1, len1, cap1);
    assert(result1 != nullptr);
    assert(result1->error == true);
    assert(result1->error_code == RSS_STRING_ERROR_STR_LEN_MISMATCH);
    free(result1);

    // Test case 2: length greater than capacity
    //
    const char* str2 = "Hello";
    size_t len2 = 5;
    size_t cap2 = 4;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == true);
    assert(result2->error_code == RSS_STRING_ERROR_LENGTH_GREATER_THAN_CAPACITY);
    free(result2);

    // Test case 3: successful creation
    //
    const char* str3 = "Hello";
    size_t len3 = 5;
    size_t cap3 = 10;
    rss_create_result* result3 = rss(str3, len3, cap3);
    assert(result3 != nullptr);
    assert(result3->error == false);
    assert(result3->error_code == RSS_STRING_ERROR_NONE);
    assert(result3->string != nullptr);
    assert(result3->string->len == len3);
    assert(result3->string->cap == cap3);
    assert(strcmp(result3->string->array, str3) == 0);
    free(result3->string->array);
    free(result3->string);
    free(result3);

    // Test case 4: empty string
    //
    const char* str4 = "";
    size_t len4 = 0;
    size_t cap4 = 5;
    rss_create_result* result4 = rss(str4, len4, cap4);
    assert(result4 != nullptr);
    assert(result4->error == false);
    assert(result4->error_code == RSS_STRING_ERROR_NONE);
    assert(result4->string != nullptr);
    assert(result4->string->len == len4);
    assert(result4->string->cap == cap4);
    assert(strcmp(result4->string->array, str4) == 0);
    free(result4->string->array);
    free(result4->string);
    free(result4);

    // Test case 5: early null terminator
    //
    const char* str5 = "He\0llo";
    size_t len5 = 5;
    size_t cap5 = 10;
    rss_create_result* result5 = rss(str5, len5, cap5);
    assert(result5 != nullptr);
    assert(result5->error == true);
    assert(result5->error_code == RSS_STRING_ERROR_STR_LEN_MISMATCH);
    free(result5);

    // Test case 6: length equal to capacity
    //
    const char* str6 = "Hello";
    size_t len6 = 5;
    size_t cap6 = 5;
    rss_create_result* result6 = rss(str6, len6, cap6);
    assert(result6 != nullptr);
    assert(result6->error == false);
    assert(result6->error_code == RSS_STRING_ERROR_NONE);
    assert(result6->string != nullptr);
    assert(result6->string->len == len6);
    assert(result6->string->cap == cap6);
    assert(strcmp(result6->string->array, str6) == 0);
    free(result6->string->array);
    free(result6->string);
    free(result6);

    // Test case 7: modification of original string after creation
    //              does not result in changes to the rss_string
    //
    char str7[] = "Hello";
    size_t len7 = 5;
    size_t cap7 = 10;
    rss_create_result* result7 = rss(str7, len7, cap7);
    assert(result7 != nullptr);
    assert(result7->error == false);
    assert(result7->error_code == RSS_STRING_ERROR_NONE);
    assert(result7->string != nullptr);
    assert(result7->string->len == len7);
    assert(result7->string->cap == cap7);
    assert(strcmp(result7->string->array, str7) == 0);

    str7[0] = 'h'; // Modify the first character of the original string
    assert(strcmp(result7->string->array, "Hello") == 0); // The rss_string should remain unchanged

    free(result7->string->array);
    free(result7->string);
    free(result7);
}


static void test_rss_string_contains_char()
{
    // Test case 1: character is present in the string
    //
    const char* str1 = "Hello";
    size_t len1 = 5;
    size_t cap1 = 10;
    rss_create_result* result1 = rss(str1, len1, cap1);
    assert(result1 != nullptr);
    assert(result1->error == false);
    assert(result1->error_code == RSS_STRING_ERROR_NONE);
    assert(result1->string != nullptr);
    assert(rss_contains_char(result1->string, 'e') == true);
    free(result1->string->array);
    free(result1->string);
    free(result1);

    // Test case 2: character is not present in the string
    //
    const char* str2 = "Hello";
    size_t len2 = 5;
    size_t cap2 = 10;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    assert(rss_contains_char(result2->string, 'z') == false);
    free(result2->string->array);
    free(result2->string);
    free(result2);

    // Test case 3: empty string
    //
    const char* str3 = "";
    size_t len3 = 0;
    size_t cap3 = 10;
    rss_create_result* result3 = rss(str3, len3, cap3);
    assert(result3 != nullptr);
    assert(result3->error == false);
    assert(result3->error_code == RSS_STRING_ERROR_NONE);
    assert(result3->string != nullptr);
    assert(rss_contains_char(result3->string, 'a') == false);
    free(result3->string->array);
    free(result3->string);
    free(result3);

    // Test case 4: handles null terminator correctly
    //
    rss_string *string4 = malloc(sizeof(rss_string));
    size_t len4 = 6;
    size_t cap4 = 10;
    string4->array = malloc(sizeof(char) * (cap4 + 1));
    string4->array[0] = 'a';
    string4->array[1] = 'b';
    string4->array[2] = RSS_NULL_TERMINATOR; // Null terminator in the middle
    string4->array[3] = 'c';
    string4->array[4] = 'd';
    string4->array[5] = 'e';
    string4->array[len4] = RSS_NULL_TERMINATOR;

    string4->len = len4;
    string4->cap = cap4;

    assert(rss_contains_char(string4, 'a') == true);
    assert(rss_contains_char(string4, 'b') == true);
    assert(rss_contains_char(string4, RSS_NULL_TERMINATOR) == true);
    assert(rss_contains_char(string4, 'c') == true);
    assert(rss_contains_char(string4, 'd') == true);
    assert(rss_contains_char(string4, 'e') == true);

    free(string4->array);
    free(string4);
}

static void test_rss_string_contains_substring()
{
    // Test case 1: substring is present in the string
    //
    const char* str1 = "Hello, world!";
    size_t len1 = 13;
    size_t cap1 = 20;
    rss_create_result* result1 = rss(str1, len1, cap1);
    assert(result1 != nullptr);
    assert(result1->error == false);
    assert(result1->error_code == RSS_STRING_ERROR_NONE);
    assert(result1->string != nullptr);
    assert(rss_contains_substring(result1->string, "world") == true);
    free(result1->string->array);
    free(result1->string);
    free(result1);

    // Test case 2: substring is not present in the string
    //
    const char* str2 = "Hello, world!";
    size_t len2 = 13;
    size_t cap2 = 20;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    assert(rss_contains_substring(result2->string, "planet") == false);
    free(result2->string->array);
    free(result2->string);
    free(result2);

    // Test case 3: empty substring
    //
    const char* str3 = "Hello, world!";
    size_t len3 = 13;
    size_t cap3 = 20;
    rss_create_result* result3 = rss(str3, len3, cap3);
    assert(result3 != nullptr);
    assert(result3->error == false);
    assert(result3->error_code == RSS_STRING_ERROR_NONE);
    assert(result3->string != nullptr);
    assert(rss_contains_substring(result3->string, "") == true);
    free(result3->string->array);
    free(result3->string);
    free(result3);

    // Test case 4: null terminator in the substring
    //              matches the null terminator in the
    //              string
    //
    rss_string *string4 = malloc(sizeof(rss_string));
    size_t len4 = 6;
    size_t cap4 = 10;
    string4->array = malloc(sizeof(char) * (cap4 + 1));
    string4->array[0] = 'a';
    string4->array[1] = 'b';
    string4->array[2] = RSS_NULL_TERMINATOR; // Null terminator in the middle
    string4->array[3] = 'c';
    string4->array[4] = 'd';
    string4->array[5] = 'e';
    string4->array[len4] = RSS_NULL_TERMINATOR;

    string4->len = len4;
    string4->cap = cap4;

    assert(rss_contains_substring(string4, "b\0c") == true);
    free(string4->array);
    free(string4);

    // Test case 5: substring `bc` does not match the
    //              string because of the null terminator
    //              in the middle
    //
    rss_string *string5 = malloc(sizeof(rss_string));
    size_t len5 = 6;
    size_t cap5 = 10;
    string5->array = malloc(sizeof(char) * (cap5 + 1));
    string5->array[0] = 'a';
    string5->array[1] = 'b';
    string5->array[2] = RSS_NULL_TERMINATOR; // Null terminator in the middle
    string5->array[3] = 'c';
    string5->array[4] = 'd';
    string5->array[5] = 'e';
    string5->array[len5] = RSS_NULL_TERMINATOR;

    string5->len = len5;
    string5->cap = cap5;

    assert(rss_contains_substring(string5, "bc") == false);
    free(string5->array);
    free(string5);
}

static void test_rss_string_length_between()
{
    // Test case 1: length is within the range
    //
    const char* str1 = "Hello";
    size_t len1 = 5;
    size_t cap1 = 10;
    rss_create_result* result1 = rss(str1, len1, cap1);
    assert(result1 != nullptr);
    assert(result1->error == false);
    assert(result1->error_code == RSS_STRING_ERROR_NONE);
    assert(result1->string != nullptr);
    assert(rss_length_between(result1->string, 3, 7) == true);
    free(result1->string->array);
    free(result1->string);
    free(result1);

    // Test case 2: length is below the range
    //
    const char* str2 = "Hello";
    size_t len2 = 5;
    size_t cap2 = 10;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    assert(rss_length_between(result2->string, 6, 10) == false);
    free(result2->string->array);
    free(result2->string);
    free(result2);

    // Test case 3: length is above the range
    //
    const char* str3 = "Hello";
    size_t len3 = 5;
    size_t cap3 = 10;
    rss_create_result* result3 = rss(str3, len3, cap3);
    assert(result3 != nullptr);
    assert(result3->error == false);
    assert(result3->error_code == RSS_STRING_ERROR_NONE);
    assert(result3->string != nullptr);
    assert(rss_length_between(result3->string, 1, 4) == false);
    free(result3->string->array);
    free(result3->string);
    free(result3);

    // Test case 4: does count null terminator
    //              in the middle of the string
    //
    rss_string *string4 = malloc(sizeof(rss_string));
    size_t len4 = 6;
    size_t cap4 = 10;
    string4->array = malloc(sizeof(char) * (cap4 + 1));
    string4->array[0] = 'a';
    string4->array[1] = 'b';
    string4->array[2] = RSS_NULL_TERMINATOR; // Null terminator
    string4->array[3] = 'c';
    string4->array[4] = 'd';
    string4->array[5] = 'e';
    string4->array[len4] = RSS_NULL_TERMINATOR;

    string4->len = len4;
    string4->cap = cap4;

    assert(rss_length_between(string4, 6, 10) == true);
    assert(rss_length_between(string4, 1, 5) == false);
    free(string4->array);
    free(string4);
}


int main()
{
    test_rss_input_matches_length();
    test_rss_result_create();
    test_rss();
    test_rss_string_contains_char();
    test_rss_string_contains_substring();
    test_rss_string_length_between();

    return 0;
}
