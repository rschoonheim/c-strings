#include "../includes/array.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief   Calculates the maximum number of elements that the array can hold
 *          based on the size of each element.
 *
 * @param   {size_t} element_size - the size of each element in the array
 *
 * @return  {size_t} - the maximum number of elements that can be stored in the array
 */
static size_t array_maximum_size(const size_t element_size)
{
    if (element_size == 0)
    {
        return 0;
    }

    return SIZE_MAX / element_size;
}

static rss_array_result* array_result_make(bool error, int error_code, rss_array* array)
{
    rss_array_result* result = malloc(sizeof(rss_array_result));
    if (result == nullptr)
    {
        return nullptr;
    }

    result->error = error;
    result->error_code = error_code;
    result->array = array;
    result->has_space = false;

    return result;
}

// Reuse the already allocated result when an operation fails.
static rss_array_result* array_result_error(rss_array_result* result, const int error_code)
{
    result->error = true;
    result->error_code = error_code;
    result->array = nullptr;
    result->has_space = false;
    return result;
}

rss_array_result* rss_array_validate(rss_array* array)
{
    if (array == nullptr)
    {
        return array_result_make(true, RSS_ARRAY_ERROR_ARRAY_NULLPTR, nullptr);
    }

    // Validate the array's internal state:
    //  - The elements pointer should not be nullptr,
    //  - The length of the array should not exceed the capacity,
    //  - The element size should not be zero,
    //
    if (array->elements == nullptr || array->len > array->cap ||
        array->element_size == 0 || array->cap == 0)
    {
        return array_result_make(true, RSS_ARRAY_ERROR_INVALID_STATE, nullptr);
    }

    if (array->cap > array_maximum_size(array->element_size))
    {
        return array_result_make(true, RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE, nullptr);
    }

    return array_result_make(false, RSS_ARRAY_ERROR_NONE, array);
}

rss_array_result* rss_array_create(const size_t element_size, const size_t cap)
{
    if (element_size == 0 || cap == 0)
    {
        return array_result_make(true, RSS_ARRAY_ERROR_INVALID_STATE, nullptr);
    }

    // Prevent overflow when calculating the total
    // size of the array
    //
    if (cap > array_maximum_size(element_size))
    {
        return array_result_make(true, RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE, nullptr);
    }

    rss_array_result* result = array_result_make(false, RSS_ARRAY_ERROR_NONE, nullptr);
    if (result == nullptr)
    {
        return nullptr;
    }

    rss_array* array = malloc(sizeof(rss_array));
    if (array == nullptr)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    array->element_size = element_size;
    array->elements = malloc(array->element_size * cap);
    if (array->elements == nullptr)
    {
        free(array);
        return array_result_error(result, RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    array->len = 0;
    array->cap = cap;

    result->array = array;
    return result;
}

rss_array_result* rss_array_destroy(rss_array* array)
{
    if (array == nullptr)
    {
        return array_result_make(true, RSS_ARRAY_ERROR_ARRAY_NULLPTR, nullptr);
    }

    rss_array_result* result = array_result_make(false, RSS_ARRAY_ERROR_NONE, nullptr);
    if (result == nullptr)
    {
        return nullptr;
    }

    free(array->elements);
    free(array);
    return result;
}

rss_array_result* rss_array_add(rss_array* array, const void* element, const size_t element_size)
{
    rss_array_result* result = rss_array_validate(array);
    if (result == nullptr || result->error)
    {
        return result;
    }

    if (element == nullptr)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_ELEMENT_NULLPTR);
    }

    // Only accept elements that matches the
    // array element size.
    //
    if (element_size != array->element_size)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_ELEMENT_SIZE_MISMATCH);
    }

    // Is there enough capacity in the
    // array to add the new element?
    //
    if (array->len >= array->cap)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_LEN_GREATER_OR_EQUAL_TO_CAP);
    }

    // Copy the element into the array
    //
    memmove(
        (unsigned char*)array->elements +
        array->len * array->element_size,
        element,
        element_size
    );

    array->len++;
    return result;
}

rss_array_result* rss_array_capacity_grow(rss_array* array, const size_t new_cap)
{
    rss_array_result* result = rss_array_validate(array);
    if (result == nullptr || result->error)
    {
        return result;
    }

    // Ensure correctness of the new capacity by ensuring that:
    //  -   The new capacity is greater than the current capacity
    //  -   The new capacity is not zero
    //
    if (new_cap <= array->cap)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_TOO_SMALL);
    }

    // Prevent overflow when calculating the total
    // size of the array
    //
    if (new_cap > array_maximum_size(array->element_size))
    {
        return array_result_error(result, RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_EXCEEDS_MAX_SIZE);
    }

    void* new_elements = realloc(array->elements, array->element_size * new_cap);
    if (new_elements == nullptr)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    array->elements = new_elements;
    array->cap = new_cap;

    return result;
}

rss_array_result* rss_array_delete(rss_array* array, const size_t index)
{
    rss_array_result* result = rss_array_validate(array);
    if (result == nullptr || result->error)
    {
        return result;
    }

    // Is the index within the bounds of the array?
    //
    if (index >= array->len)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_INDEX_OUT_OF_BOUNDS);
    }

    // Shift all elements after the index 1 position, this will
    // overwrite (delete) the element at the index and move all
    // subsequent elements down by 1.
    //
    memmove(
        (unsigned char*)array->elements + index * array->element_size,
        (unsigned char*)array->elements + (index + 1) * array->element_size,
        (array->len - index - 1) * array->element_size
    );
    array->len--;

    // Clear the duplicate bytes in the newly vacated slot.
    //
    memset(
        (unsigned char*)array->elements + array->len * array->element_size,
        0,
        array->element_size
    );

    return result;
}

rss_array_result* rss_array_has_space(rss_array* array)
{
    rss_array_result* result = rss_array_validate(array);
    if (result == nullptr || result->error)
    {
        return result;
    }

    result->has_space = array->len < array->cap;
    return result;
}

rss_array_result* rss_array_compress(rss_array* array)
{
    rss_array_result* result = rss_array_validate(array);
    if (result == nullptr || result->error)
    {
        return result;
    }

    // Never pass a zero allocation size to realloc.
    if (array->len == 0)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_ARRAY_EMPTY);
    }

    void* new_elements = realloc(array->elements, array->element_size * array->len);
    if (new_elements == nullptr)
    {
        return array_result_error(result, RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED);
    }

    // Update the array's elements with the
    // new elements and set the capacity to
    // the length of the array.
    //
    array->elements = new_elements;
    array->cap = array->len;

    return result;
}
