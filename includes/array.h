#ifndef ROMANO_SCHOONHEIM_C_STRINGS_ARRAY_H
#define ROMANO_SCHOONHEIM_C_STRINGS_ARRAY_H
#include <stddef.h>

typedef struct rss_array
{
    void* elements;
    size_t element_size;
    size_t len;
    size_t cap;
} rss_array;

/**
 * @brief   When receiving an array from "the outside world", we need to verify
 *          that the array is valid and that it can be used safely. This function
 *          checks the validity of the array and returns true if it is valid,
 *          false otherwise.
 *
 *          Invalid state of arrays is caused by external factors, such as:
 *              - Changes to the array structure from outside the library,
 *              - Corruption of the array structure in memory,
 *              - Invalid memory access to the array structure.
 *
 * @param   {rss_array*} array - the rss_array to check
 *
 * @return  {bool} - true if the array is valid, false otherwise
 */
bool rss_array_validate(const rss_array* array);

/**
 * @brief   Destroys the given rss_array and frees its memory.
 *
 * @param   {rss_array*} array - the rss_array to be destroyed
 *
 * @return  {bool} - true if the array was successfully destroyed, false otherwise
 */
bool rss_array_destroy(rss_array* array);

/**
 * @brief   Creates a new rss_array with enough space to hold `cap`
 *          elements of size `element_size`.
 *
 * @param   {size_t} element_size - the size of each element in the array
 * @param   {size_t} cap - the capacity of the array
 *
 * @return  {rss_array*} - a pointer to the created rss_array, or nullptr if allocation fails
 */
rss_array* rss_array_create(size_t element_size, size_t cap);

/**
 * @brief   Adds an element to the end of the array.
 *
 * @param   {rss_array*} array - the rss_array to add the element to
 * @param   {void*} element - a pointer to the element to be added
 * @param   {size_t} element_size - the size of the element to be added
 *
 * @return  {bool} - true if the element was successfully added, false otherwise
 */
bool rss_array_add(rss_array* array, const void* element, size_t element_size);

/**
 * @brief   Grow the capacity of the array to the new capacity.
 *
 * @param   {rss_array*} array - the rss_array to update the capacity of
 * @param   {size_t} new_cap - the new capacity of the array
 *
 * @return  {bool} - true if the capacity was successfully updated, false otherwise
 */
bool rss_array_capacity_grow(rss_array* array, size_t new_cap);

/**
 * @brief   Delete an element from the array at the given index.
 *
 * @param   {rss_array*} array - the rss_array to delete the element from
 * @param   {size_t} index - the index of the element to delete
 *
 * @return  {bool} - true if the element was successfully deleted, false otherwise
 */
bool rss_array_delete(rss_array* array, size_t index);

/**
 * @brief   Does the array have space for more elements?
 *          Space is available if the length of the array
 *          is less than the capacity of the array.
 *
 * @param   {const rss_array*} array - the rss_array to check for available space
 *
 * @return  {bool} - true if the array has space for more elements, false otherwise
 */
bool rss_array_has_space(const rss_array* array);

/**
 * @brief   Reduce memory usage by compressing the capacity of the array
 *          to match the current length of the array.
 *
 * @param   {rss_array*} array - the rss_array to compress the capacity of
 *
 * @return  {bool} - true if the capacity was successfully compressed, false otherwise
 */
bool rss_array_compress(rss_array* array);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_ARRAY_H
