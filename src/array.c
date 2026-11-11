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
 * @return  {int} - the maximum number of elements that can be stored in the array
 */
static size_t array_maximum_size(const size_t element_size)
{
    if (element_size == 0)
    {
        return 0;
    }

    return SIZE_MAX / element_size;
}

bool rss_array_validate(const rss_array* array)
{
    if (array == nullptr)
    {
        return false;
    }

    // Validate the array's internal state:
    //  - The elements pointer should not be nullptr,
    //  - The length of the array should not exceed the capacity,
    //  - The element size should not be zero,
    //
    if (array->elements == nullptr || array->len > array->cap || array->element_size == 0)
    {
        return false;
    }

    return true;
}

rss_array* rss_array_create(const size_t element_size, const size_t cap)
{
    if (element_size == 0 || cap == 0)
    {
        return nullptr;
    }

    // Prevent overflow when calculating the total
    // size of the array
    //
    if (cap > array_maximum_size(element_size))
    {
        return nullptr;
    }

    rss_array* array = malloc(sizeof(rss_array));
    if (array == nullptr)
    {
        return nullptr;
    }

    array->element_size = element_size;
    array->elements = malloc(array->element_size * cap);
    if (array->elements == nullptr)
    {
        free(array);
        return nullptr;
    }

    array->len = 0;
    array->cap = cap;

    return array;
}

bool rss_array_destroy(rss_array* array)
{
    if (array == nullptr)
    {
        return false;
    }

    if (array->elements != nullptr)
    {
        free(array->elements);
    }

    free(array);
    return true;
}


bool rss_array_add(rss_array* array, const void* element, const size_t element_size)
{
    if (!rss_array_validate(array))
    {
        return false;
    }

    if (element == nullptr)
    {
        return false;
    }

    // Only accept elements that matches the
    // array element size.
    //
    if (element_size != array->element_size)
    {
        return false;
    }

    // Is there enough capacity in the
    // array to add the new element?
    //
    if (array->len >= array->cap)
    {
        return false;
    }

    // Copy the element into the array
    //
    memmove(
        (unsigned char*)array->elements +
        array->len * array->element_size,
        element, element_size
    );

    array->len++;
    return true;
}

bool rss_array_capacity_grow(rss_array* array, const size_t new_cap)
{
    if (!rss_array_validate(array))
    {
        return false;
    }

    // Ensure correctness of the new capacity by ensuring that:
    //  -   The new capacity is greater than the current capacity
    //  -   The new capacity is not zero
    //
    if (new_cap <= array->cap || new_cap == 0)
    {
        return false;
    }

    // Prevent overflow when calculating the total
    // size of the array
    //
    if (new_cap > array_maximum_size(array->element_size))
    {
        return false;
    }

    // Can the new capacity fit the current
    // length of the array?
    //
    if (new_cap < array->len)
    {
        return false;
    }

    void* new_elements = realloc(array->elements, array->element_size * new_cap);
    if (new_elements == nullptr)
    {
        return false;
    }

    array->elements = new_elements;
    array->cap = new_cap;

    return true;
}

bool rss_array_delete(rss_array* array, const size_t index)
{
    if (!rss_array_validate(array))
    {
        return false;
    }

    // Is the index within the bounds of the array?
    //
    if (index >= array->len)
    {
        return false;
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

    // Reset the gap between the last element and
    // the array capacity with zeroes to avoid
    // dangling pointers.
    //
    memset(
        (unsigned char*)array->elements + array->len * array->element_size,
        0,
        (array->cap - array->len) * array->element_size
    );

    return true;
}

bool rss_array_has_space(const rss_array* array)
{
    if (!rss_array_validate(array))
    {
        return false;
    }

    return array->len < array->cap;
}

bool rss_array_compress(rss_array* array)
{
    if (!rss_array_validate(array) || array->element_size == 0)
    {
        return false;
    }

    // Rejection of invalid array states:
    //  - The elements pointer should not be nullptr,
    //  - The length of the array should not exceed the capacity,
    //  - The length of the array should be greater than 0,
    //    cap 0 means the array should not exist,
    //
    if (array->elements == nullptr || array->len > array->cap || array->len <= 0)
    {
        return false;
    }

    // Array length cannot exceed the maximum size of the
    // array based on the element size, otherwise realloc
    // will fail and return nullptr.
    //
    if (array->len > array_maximum_size(array->element_size))
    {
        return false;
    }

    void* new_elements = realloc(array->elements, array->element_size * array->len);
    if (new_elements == nullptr)
    {
        return false;
    }

    // Update the array's elements with the
    // new elements and set the capacity to
    // the length of the array.
    //
    array->elements = new_elements;
    array->cap = array->len;

    return true;
}