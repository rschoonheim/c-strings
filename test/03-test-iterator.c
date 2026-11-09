#include "../src/iterator.c"

#include <assert.h>
#include <stdio.h>

static void test_rss_string_iterator_create()
{
    // Test case 1: successful creation
    //
    const char* str1 = "Hello";
    size_t len1 = 5;
    size_t cap1 = 10;
    rss_create_result* result1 = rss(str1, len1, cap1);
    assert(result1 != nullptr);
    assert(result1->error == false);
    assert(result1->error_code == RSS_STRING_ERROR_NONE);
    assert(result1->string != nullptr);
    rss_string_iterator_create_result* iterator_result1 = rss_string_iterator_create(result1->string);
    assert(iterator_result1 != nullptr);
    assert(iterator_result1->error == false);
    assert(iterator_result1->error_code == RSS_STRING_ITERATOR_ERROR_NONE);
    assert(iterator_result1->iterator != nullptr);
    assert(iterator_result1->iterator->string == result1->string);
    assert(iterator_result1->iterator->current_index == 0);
    free(iterator_result1->iterator);
    free(iterator_result1);

    // Test case 2: string is nullptr
    //
    rss_string_iterator_create_result* iterator_result2 = rss_string_iterator_create(nullptr);
    assert(iterator_result2 != nullptr);
    assert(iterator_result2->error == true);
    assert(iterator_result2->error_code == RSS_STRING_ITERATOR_ERROR_STRING_NULL);
    free(iterator_result2);

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
    rss_string_iterator_create_result* iterator_result3 = rss_string_iterator_create(result3->string);
    assert(iterator_result3 != nullptr);
    assert(iterator_result3->error == false);
    assert(iterator_result3->error_code == RSS_STRING_ITERATOR_ERROR_NONE);
    assert(iterator_result3->iterator != nullptr);
    assert(iterator_result3->iterator->string == result3->string);
    assert(iterator_result3->iterator->current_index == 0);
    free(iterator_result3->iterator);
    free(iterator_result3);
    free(result3->string->array);
    free(result3->string);
}

static void test_rss_string_iterator_destroy()
{
    // Test case 1: nullptr iterator
    //
    assert(rss_string_iterator_destroy(nullptr, false) == false);

    // Test case 2: valid iterator
    //
    const char* str2 = "Hello";
    size_t len2 = 5;
    size_t cap2 = 10;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    rss_string_iterator_create_result* iterator_result2 = rss_string_iterator_create(result2->string);
    assert(iterator_result2 != nullptr);
    assert(iterator_result2->error == false);
    assert(iterator_result2->error_code == RSS_STRING_ITERATOR_ERROR_NONE);

    assert(rss_string_iterator_destroy(iterator_result2->iterator, false) == true);
}

static void test_rss_string_iterator_at_end()
{
    // Test case 1: nullptr iterator
    //
    assert(rss_string_iterator_at_end(nullptr) == true);

    // Test case 2: valid iterator
    //
    const char* str2 = "Hello";
    size_t len2 = 5;
    size_t cap2 = 10;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    rss_string_iterator_create_result* iterator_result2 = rss_string_iterator_create(result2->string);
    assert(iterator_result2 != nullptr);
    assert(iterator_result2->error == false);
    assert(iterator_result2->error_code == RSS_STRING_ITERATOR_ERROR_NONE);

    assert(rss_string_iterator_at_end(iterator_result2->iterator) == false);

    free(iterator_result2->iterator);
    free(iterator_result2);

    // Test case 3: iterator at end
    //
    const char* str3 = "Hello";
    size_t len3 = 5;
    size_t cap3 = 10;
    rss_create_result* result3 = rss(str3, len3, cap3);
    assert(result3 != nullptr);
    assert(result3->error == false);
    assert(result3->error_code == RSS_STRING_ERROR_NONE);
    assert(result3->string != nullptr);
    rss_string_iterator_create_result* iterator_result3 = rss_string_iterator_create(result3->string);
    assert(iterator_result3 != nullptr);
    assert(iterator_result3->error == false);
    assert(iterator_result3->error_code == RSS_STRING_ITERATOR_ERROR_NONE);

    iterator_result3->iterator->current_index = len3;
    assert(rss_string_iterator_at_end(iterator_result3->iterator) == true);

    free(iterator_result3->iterator);
    free(iterator_result3);
}

static void test_rss_string_iterator_index_move()
{
    // Test case 1: nullptr iterator
    //
    assert(rss_string_iterator_index_move(nullptr, 1) == false);

    // Test case 2: valid iterator
    //
    const char* str2 = "Hello";
    size_t len2 = 5;
    size_t cap2 = 10;
    rss_create_result* result2 = rss(str2, len2, cap2);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_STRING_ERROR_NONE);
    assert(result2->string != nullptr);
    rss_string_iterator_create_result* iterator_result2 = rss_string_iterator_create(result2->string);
    assert(iterator_result2 != nullptr);
    assert(iterator_result2->error == false);
    assert(iterator_result2->error_code == RSS_STRING_ITERATOR_ERROR_NONE);

    assert(rss_string_iterator_index_move(iterator_result2->iterator, 3) == true);
    assert(iterator_result2->iterator->current_index == 3);

    free(iterator_result2->iterator);
    free(iterator_result2);

    // Test case 3: cannot move below 0
    //
    const char* str3 = "Hello";
    size_t len3 = 5;
    size_t cap3 = 10;
    rss_create_result* result3 = rss(str3, len3, cap3);
    assert(result3 != nullptr);
    assert(result3->error == false);
    assert(result3->error_code == RSS_STRING_ERROR_NONE);
    assert(result3->string != nullptr);
    rss_string_iterator_create_result* iterator_result3 = rss_string_iterator_create(result3->string);
    assert(iterator_result3 != nullptr);
    assert(iterator_result3->error == false);
    assert(iterator_result3->error_code == RSS_STRING_ITERATOR_ERROR_NONE);

    assert(rss_string_iterator_index_move(iterator_result3->iterator, -6) == false);
    assert(iterator_result3->iterator->current_index == 0);

    assert(rss_string_iterator_index_move(iterator_result3->iterator, 3) == true);
    assert(iterator_result3->iterator->current_index == 3);

    assert(rss_string_iterator_index_move(iterator_result3->iterator, -3) == true);
    assert(iterator_result3->iterator->current_index == 0);

    free(iterator_result3->iterator);
    free(iterator_result3);

    // Test case 4: cannot move beyond string length
    //
    const char* str4 = "Hello";
    size_t len4 = 5;
    size_t cap4 = 10;
    rss_create_result* result4 = rss(str4, len4, cap4);
    assert(result4 != nullptr);
    assert(result4->error == false);
    assert(result4->error_code == RSS_STRING_ERROR_NONE);
    assert(result4->string != nullptr);
    rss_string_iterator_create_result* iterator_result4 = rss_string_iterator_create(result4->string);
    assert(iterator_result4 != nullptr);
    assert(iterator_result4->error == false);
    assert(iterator_result4->error_code == RSS_STRING_ITERATOR_ERROR_NONE);

    assert(rss_string_iterator_index_move(iterator_result4->iterator, 6) == false);
    assert(iterator_result4->iterator->current_index == 0);

    assert(rss_string_iterator_index_move(iterator_result4->iterator, 5) == true);
    assert(iterator_result4->iterator->current_index == 5);

    assert(rss_string_iterator_index_move(iterator_result4->iterator, -5) == true);
    assert(iterator_result4->iterator->current_index == 0);

    free(iterator_result4->iterator);
    free(iterator_result4);
}

int main()
{
    test_rss_string_iterator_create();
    test_rss_string_iterator_destroy();
    test_rss_string_iterator_at_end();
    test_rss_string_iterator_index_move();

    return 0;
}