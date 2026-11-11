#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// Inject failures only into allocations made by the array implementation.
static size_t allocations_before_failure = SIZE_MAX;
static bool fail_reallocation = false;

static void* test_malloc(size_t size)
{
    assert(size > 0);
    if (allocations_before_failure == 0)
    {
        return nullptr;
    }
    if (allocations_before_failure != SIZE_MAX)
    {
        allocations_before_failure--;
    }
    return malloc(size);
}

static void* test_realloc(void* pointer, size_t size)
{
    assert(size > 0);
    return fail_reallocation ? nullptr : realloc(pointer, size);
}

#define malloc test_malloc
#define realloc test_realloc
#include "../src/array.c"
#undef malloc
#undef realloc

static void expect_success(rss_array_result* result, rss_array* array)
{
    assert(result != nullptr);
    assert(!result->error);
    assert(result->error_code == RSS_ARRAY_ERROR_NONE);
    assert(result->array == array);
    free(result);
}

static void expect_error(rss_array_result* result, int error_code)
{
    assert(result != nullptr);
    assert(result->error);
    assert(result->error_code == error_code);
    assert(result->array == nullptr);
    assert(!result->has_space);
    free(result);
}

static rss_array* create_array(size_t cap)
{
    rss_array_result* result = rss_array_create(sizeof(int), cap);
    assert(result != nullptr);
    assert(!result->error);
    rss_array* array = result->array;
    assert(array != nullptr);
    expect_success(result, array);
    return array;
}

static void destroy_created(rss_array_result* result)
{
    assert(result != nullptr);
    assert(!result->error);
    expect_success(rss_array_destroy(result->array), nullptr);
    free(result);
}

static void test_array_maximum_size()
{
    // Test case 1: zero element size
    //
    assert(array_maximum_size(0) == 0);

    // Test case 2: element_size is 0
    //
    rss_array array2 = { nullptr, 0, 0, 10 };
    assert(array_maximum_size(array2.element_size) == 0);

    // Test case 3: normal case
    //
    rss_array_result* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    size_t expected_max_size = SIZE_MAX / sizeof(int);
    assert(array_maximum_size(array3->array->element_size) == expected_max_size);
    assert(array_maximum_size(1) == SIZE_MAX);
    assert(array_maximum_size(SIZE_MAX) == 1);
    destroy_created(array3);
}

static void test_rss_array_validate()
{
    // Test case 1: nullptr array
    //
    rss_array_result * result1 = rss_array_validate(nullptr);
    assert(result1 != nullptr);
    assert(result1->error == true);
    assert(result1->error_code == RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    free(result1);
    // Test case 2: valid array
    //
    rss_array_result* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    rss_array_result * result2 = rss_array_validate(array2->array);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_ARRAY_ERROR_NONE);
    free(result2);
    free(array2->array->elements);
    free(array2->array);
    free(array2);

    // Test case 3: invalid array (elements is nullptr)
    //
    rss_array array3 = { nullptr, sizeof(int), 0, 10 };
    rss_array_result * result3 = rss_array_validate(&array3);
    assert(result3 != nullptr);
    assert(result3->error == true);
    assert(result3->error_code == RSS_ARRAY_ERROR_INVALID_STATE);
    free(result3);

    // Test case 4: invalid array (len > cap)
    //
    rss_array array4 = { malloc(sizeof(int) * 10), sizeof(int), 11, 10 };
    rss_array_result * result4 = rss_array_validate(&array4);
    assert(result4 != nullptr);
    assert(result4->error == true);
    assert(result4->error_code == RSS_ARRAY_ERROR_INVALID_STATE);
    free(result4);
    free(array4.elements);

    // Test case 5: invalid array (element_size is 0)
    //
    rss_array array5 = { malloc(sizeof(int) * 10), 0, 0, 10 };
    rss_array_result * result5 = rss_array_validate(&array5);
    assert(result5 != nullptr);
    assert(result5->error == true);
    assert(result5->error_code == RSS_ARRAY_ERROR_INVALID_STATE);
    free(result5);
    free(array5.elements);

    int element = 1;
    rss_array zero_capacity = { &element, sizeof(int), 0, 0 };
    expect_error(rss_array_validate(&zero_capacity), RSS_ARRAY_ERROR_INVALID_STATE);
    rss_array overflowing = { &element, 2, 0, SIZE_MAX };
    expect_error(rss_array_validate(&overflowing), RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE);
}

static void test_rss_array_destroy()
{
    // Test case 1: nullptr array
    //
    rss_array_result* result1 = rss_array_destroy(nullptr);
    assert(result1 != nullptr);
    assert(result1->error == true);
    assert(result1->error_code == RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    free(result1);

    // Test case 2: valid array
    //
    rss_array_result* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    rss_array_result* result2 = rss_array_destroy(array2->array);
    assert(result2 != nullptr);
    assert(result2->error == false);
    assert(result2->error_code == RSS_ARRAY_ERROR_NONE);
    assert(result2->array == nullptr);
    free(result2);
    free(array2);
}

static void test_rss_array_create()
{
    // Test case 1: element_size is 0
    //
    rss_array_result* result1 = rss_array_create(0, 10);
    assert(result1 != nullptr);
    assert(result1->error == true);
    assert(result1->error_code == RSS_ARRAY_ERROR_INVALID_STATE);
    free(result1);

    // Test case 2: cap is 0
    //
    rss_array_result* result2 = rss_array_create(sizeof(int), 0);
    assert(result2 != nullptr);
    assert(result2->error == true);
    assert(result2->error_code == RSS_ARRAY_ERROR_INVALID_STATE);
    free(result2);

    // Test case 3: cap exceeds maximum size
    //
    rss_array_result* result3 = rss_array_create(sizeof(int), SIZE_MAX / sizeof(int) + 1);
    assert(result3 != nullptr);
    assert(result3->error == true);
    assert(result3->error_code == RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE);
    free(result3);
}

static void test_rss_array_add()
{
    // Test case 1: nullptr array
    //
    int element1 = 42;
    rss_array_result* result1 = rss_array_add(nullptr, &element1, sizeof(int));
    assert(result1 != nullptr);
    assert(result1->error == true);
    assert(result1->error_code == RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    free(result1);

    // Test case 2: element is nullptr
    //
    rss_array_result* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    rss_array_result* result2 = rss_array_add(array2->array, nullptr, sizeof(int));
    assert(result2 != nullptr);
    assert(result2->error == true);
    assert(result2->error_code == RSS_ARRAY_ERROR_ELEMENT_NULLPTR);
    free(result2);
    destroy_created(array2);

    // Test case 3: element size mismatch
    //
    rss_array_result* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    double element3 = 3.14;
    rss_array_result* result3 = rss_array_add(array3->array, &element3, sizeof(double));
    assert(result3 != nullptr);
    assert(result3->error == true);
    assert(result3->error_code == RSS_ARRAY_ERROR_ELEMENT_SIZE_MISMATCH);
    free(result3);
    destroy_created(array3);

    // Test case 4: no capacity left in the array
    //
    rss_array_result* array4 = rss_array_create(sizeof(int), 1);
    assert(array4 != nullptr);
    int element4a = 42;
    rss_array_result* result4a = rss_array_add(array4->array, &element4a, sizeof(int));
    assert(result4a != nullptr);
    assert(result4a->error == false);
    int element4b = 43;
    rss_array_result* result4b = rss_array_add(array4->array, &element4b, sizeof(int));
    assert(result4b != nullptr);
    assert(result4b->error == true);
    assert(result4b->error_code == RSS_ARRAY_ERROR_LEN_GREATER_OR_EQUAL_TO_CAP);
    free(result4a);
    free(result4b);
    destroy_created(array4);

    // Test case 5: successful addition
    //
    rss_array_result* array5 = rss_array_create(sizeof(int), 10);
    assert(array5 != nullptr);
    int element5 = 42;
    rss_array_result* result5 = rss_array_add(array5->array, &element5, sizeof(int));
    assert(result5 != nullptr);
    assert(result5->error == false);
    assert(result5->error_code == RSS_ARRAY_ERROR_NONE);
    assert(result5->array == array5->array);
    assert(((int*)array5->array->elements)[0] == 42);
    free(result5);
    // A source within the array is valid; memmove supports overlap.
    expect_success(rss_array_add(array5->array, array5->array->elements, sizeof(int)), array5->array);
    assert(array5->array->len == 2);
    assert(((int*)array5->array->elements)[1] == 42);
    destroy_created(array5);
}

static void test_rss_array_capacity_grow()
{
    // Test case 1: nullptr array
    //
    rss_array_result *result1 = rss_array_capacity_grow(nullptr, 10);
    assert(result1 != nullptr);
    assert(result1->error == true);
    assert(result1->error_code == RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    free(result1);

    // Test case 2: new capacity is less than current capacity
    //
    rss_array_result* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    rss_array_result *result2 = rss_array_capacity_grow(array2->array, 5);
    assert(result2 != nullptr);
    assert(result2->error == true);
    assert(result2->error_code == RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_TOO_SMALL);
    free(result2);
    expect_error(rss_array_capacity_grow(array2->array, 0),
                 RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_TOO_SMALL);
    destroy_created(array2);

    // Test case 3: new capacity is equal to current capacity
    //
    rss_array_result* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    rss_array_result *result3 = rss_array_capacity_grow(array3->array, 10);
    assert(result3 != nullptr);
    assert(result3->error == true);
    assert(result3->error_code == RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_TOO_SMALL);
    free(result3);
    destroy_created(array3);

    // Test case 4: new capacity exceeds maximum size
    //
    rss_array_result* array4 = rss_array_create(sizeof(int), 10);
    assert(array4 != nullptr);
    size_t new_cap4 = SIZE_MAX / sizeof(int) + 1;
    rss_array_result *result4 = rss_array_capacity_grow(array4->array, new_cap4);
    assert(result4 != nullptr);
    assert(result4->error == true);
    assert(result4->error_code == RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_EXCEEDS_MAX_SIZE);
    free(result4);
    destroy_created(array4);

    // Test case 5: successful capacity growth
    //
    rss_array_result* array5 = rss_array_create(sizeof(int), 10);
    assert(array5 != nullptr);
    size_t new_cap5 = 20;
    rss_array_result *result5 = rss_array_capacity_grow(array5->array, new_cap5);
    assert(result5 != nullptr);
    assert(result5->error == false);
    assert(result5->error_code == RSS_ARRAY_ERROR_NONE);
    assert(result5->array == array5->array);
    assert(result5->array->cap == new_cap5);
    free(result5);
    destroy_created(array5);

    // Test case 6: elements are preserved after capacity growth
    //
    rss_array_result* array6 = rss_array_create(sizeof(int), 10);
    assert(array6 != nullptr);
    int element6a = 42;
    rss_array_result* add_result6a = rss_array_add(array6->array, &element6a, sizeof(int));
    assert(add_result6a != nullptr);
    assert(add_result6a->error == false);
    size_t new_cap6 = 20;
    rss_array_result *result6 = rss_array_capacity_grow(array6->array, new_cap6);
    assert(result6 != nullptr);
    assert(result6->error == false);
    assert(((int*)array6->array->elements)[0] == 42);
    free(add_result6a);
    free(result6);
    free(array6->array->elements);
    free(array6->array);
    free(array6);
}

static void test_rss_array_delete()
{
    expect_error(rss_array_delete(nullptr, 0), RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    rss_array* array = create_array(4);
    expect_error(rss_array_delete(array, 0), RSS_ARRAY_ERROR_INDEX_OUT_OF_BOUNDS);
    for (int value = 1; value <= 4; value++)
    {
        expect_success(rss_array_add(array, &value, sizeof(value)), array);
    }
    expect_error(rss_array_delete(array, array->len), RSS_ARRAY_ERROR_INDEX_OUT_OF_BOUNDS);
    expect_error(rss_array_delete(array, SIZE_MAX), RSS_ARRAY_ERROR_INDEX_OUT_OF_BOUNDS);
    assert(array->len == 4);

    expect_success(rss_array_delete(array, 1), array);
    int* elements = array->elements;
    assert(array->len == 3 && array->cap == 4);
    assert(elements[0] == 1 && elements[1] == 3 && elements[2] == 4);
    assert(elements[3] == 0);
    expect_success(rss_array_delete(array, 2), array);
    assert(array->len == 2 && elements[2] == 0);
    expect_success(rss_array_delete(array, 0), array);
    assert(array->len == 1 && elements[0] == 3 && elements[1] == 0);
    expect_success(rss_array_delete(array, 0), array);
    assert(array->len == 0 && elements[0] == 0);
    expect_success(rss_array_destroy(array), nullptr);
}

static void test_rss_array_has_space()
{
    expect_error(rss_array_has_space(nullptr), RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    rss_array* array = create_array(1);
    rss_array_result* result = rss_array_has_space(array);
    assert(result != nullptr && result->has_space);
    expect_success(result, array);
    int value = 42;
    expect_success(rss_array_add(array, &value, sizeof(value)), array);
    result = rss_array_has_space(array);
    assert(result != nullptr && !result->has_space);
    expect_success(result, array); // Full is a successful query, not an error.
    expect_success(rss_array_delete(array, 0), array);
    result = rss_array_has_space(array);
    assert(result != nullptr && result->has_space);
    expect_success(result, array);
    expect_success(rss_array_destroy(array), nullptr);
}

static void test_rss_array_compress()
{
    expect_error(rss_array_compress(nullptr), RSS_ARRAY_ERROR_ARRAY_NULLPTR);
    rss_array* array = create_array(8);
    expect_error(rss_array_compress(array), RSS_ARRAY_ERROR_ARRAY_EMPTY);
    assert(array->cap == 8 && array->len == 0);
    for (int value = 1; value <= 3; value++)
    {
        expect_success(rss_array_add(array, &value, sizeof(value)), array);
    }
    expect_success(rss_array_compress(array), array);
    assert(array->cap == 3 && array->len == 3);
    int* elements = array->elements;
    assert(elements[0] == 1 && elements[1] == 2 && elements[2] == 3);
    expect_success(rss_array_compress(array), array);
    assert(array->cap == 3 && array->len == 3);
    expect_success(rss_array_destroy(array), nullptr);
}

static void test_invalid_state_operations()
{
    int value = 42;
    rss_array invalid[] = {
        { nullptr, sizeof(int), 0, 1 },
        { &value, sizeof(int), 2, 1 },
        { &value, 0, 0, 1 },
        { &value, sizeof(int), 0, 0 },
        { &value, 2, 0, SIZE_MAX }
    };
    for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); i++)
    {
        rss_array* array = &invalid[i];
        int code = i == 4 ? RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE : RSS_ARRAY_ERROR_INVALID_STATE;
        expect_error(rss_array_validate(array), code);
        expect_error(rss_array_add(array, &value, sizeof(value)), code);
        expect_error(rss_array_capacity_grow(array, 10), code);
        expect_error(rss_array_delete(array, 0), code);
        expect_error(rss_array_has_space(array), code);
        expect_error(rss_array_compress(array), code);
    }
    assert(value == 42);
}

static void test_allocation_failures()
{
    // Fail each allocation in creation: result, array structure, then buffer.
    for (size_t i = 0; i < 3; i++)
    {
        allocations_before_failure = i;
        rss_array_result* result = rss_array_create(sizeof(int), 4);
        allocations_before_failure = SIZE_MAX;
        if (i == 0)
        {
            assert(result == nullptr);
        }
        else
        {
            expect_error(result, RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
        }
    }

    rss_array* array = create_array(4);
    int value = 42;
    expect_success(rss_array_add(array, &value, sizeof(value)), array);
    void* original_elements = array->elements;

    // Every operation must tolerate failure to allocate its result.
    allocations_before_failure = 0;
    assert(rss_array_validate(array) == nullptr);
    assert(rss_array_add(array, &value, sizeof(value)) == nullptr);
    assert(rss_array_capacity_grow(array, 8) == nullptr);
    assert(rss_array_delete(array, 0) == nullptr);
    assert(rss_array_has_space(array) == nullptr);
    assert(rss_array_compress(array) == nullptr);
    assert(rss_array_destroy(array) == nullptr);
    assert(rss_array_validate(nullptr) == nullptr);
    assert(rss_array_create(0, 1) == nullptr);
    assert(rss_array_destroy(nullptr) == nullptr);
    allocations_before_failure = SIZE_MAX;

    assert(array->elements == original_elements);
    assert(array->len == 1 && array->cap == 4);
    assert(((int*)array->elements)[0] == value);

    fail_reallocation = true;
    expect_error(rss_array_capacity_grow(array, 8), RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
    expect_error(rss_array_compress(array), RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
    fail_reallocation = false;
    assert(array->elements == original_elements);
    assert(array->len == 1 && array->cap == 4);
    assert(((int*)array->elements)[0] == value);

    // No result allocation may occur after a successful mutation.
    allocations_before_failure = 1;
    expect_success(rss_array_add(array, &value, sizeof(value)), array);
    allocations_before_failure = 1;
    expect_success(rss_array_capacity_grow(array, 8), array);
    allocations_before_failure = 1;
    expect_success(rss_array_delete(array, 0), array);
    allocations_before_failure = 1;
    expect_success(rss_array_compress(array), array);
    assert(array->len == 1 && array->cap == 1);
    assert(((int*)array->elements)[0] == value);
    allocations_before_failure = 1;
    expect_success(rss_array_destroy(array), nullptr);
    allocations_before_failure = SIZE_MAX;
}

int main()
{
    test_array_maximum_size();
    test_rss_array_validate();
    test_rss_array_destroy();
    test_rss_array_create();
    test_rss_array_add();
    test_rss_array_capacity_grow();

    test_rss_array_delete();
    test_rss_array_has_space();
    test_rss_array_compress();
    test_invalid_state_operations();
    test_allocation_failures();

    return 0;
}