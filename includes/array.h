#ifndef ROMANO_SCHOONHEIM_C_STRINGS_ARRAY_H
#define ROMANO_SCHOONHEIM_C_STRINGS_ARRAY_H
#include <stddef.h>

#define RSS_ARRAY_ERROR_NONE 0
#define RSS_ARRAY_ERROR_ARRAY_NULLPTR 1
#define RSS_ARRAY_ERROR_INVALID_STATE 2
#define RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE 3
#define RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED 4
#define RSS_ARRAY_ERROR_ELEMENT_NULLPTR 5
#define RSS_ARRAY_ERROR_ELEMENT_SIZE_MISMATCH 6
#define RSS_ARRAY_ERROR_LEN_GREATER_OR_EQUAL_TO_CAP 7
#define RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_TOO_SMALL 8
#define RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_EXCEEDS_MAX_SIZE 9
#define RSS_ARRAY_ERROR_INDEX_OUT_OF_BOUNDS 10
#define RSS_ARRAY_ERROR_ARRAY_EMPTY 11

typedef struct rss_array
{
    void* elements;
    size_t element_size;
    size_t len;
    size_t cap;
} rss_array;

/**
 * Every non-null result is owned by the caller and must be freed with free().
 * Freeing a result does not destroy its array. On success, array is a borrowed
 * mutable handle (except after destroy, when it is nullptr). On error it is
 * nullptr. has_space is the query value only for rss_array_has_space().
 *
 * All operations return nullptr if allocating the result fails; in that case
 * the array is unchanged. Other failures return error == true with an error
 * code, also leaving the array unchanged. Successful operations use ERROR_NONE.
 * Array handles must be live and exclusively owned, with synchronized access.
 */
typedef struct rss_array_result
{
    bool error;
    int error_code;
    rss_array* array;
    bool has_space;
} rss_array_result;

/**
 * @brief   Checks array metadata without modifying the array. This cannot
 *          verify pointer lifetime, ownership or the actual allocation size.
 *
 * @param   {rss_array*} array - the rss_array to check
 *
 * @return  {rss_array_result *} - a pointer to the rss_array_result structure containing the validation result
 */
rss_array_result* rss_array_validate(rss_array* array);

/**
 * @brief   Destroys the given rss_array and frees its memory.
 *          Stored pointer values are not followed or freed. All array/buffer
 *          aliases become invalid on success; the result's array is nullptr.
 *
 * @param   {rss_array*} array - the rss_array to be destroyed
 *
 * @return  {rss_array_result * } - a pointer to the rss_array_result structure containing the result of the destruction
 */
rss_array_result* rss_array_destroy(rss_array* array);

/**
 * @brief   Creates a new rss_array with enough space to hold `cap`
 *          elements of size `element_size`.
 *          Both sizes must be nonzero. The caller owns the created array.
 *
 * @param   {size_t} element_size - the size of each element in the array
 * @param   {size_t} cap - the capacity of the array
 *
 * @return  {rss_array_result *} - a pointer to the rss_array_result structure containing the result of the creation
 */
rss_array_result* rss_array_create(size_t element_size, size_t cap);

/**
 * @brief   Adds an element to the end of the array.
 *
 * @param   {rss_array*} array - the rss_array to add the element to
 * @param   {const void*} element - at least element_size readable source bytes
 * @param   {size_t} element_size - the size of the element to be added
 *
 * @return  {rss_array_result * } - a pointer to the rss_array_result structure containing the result of the addition
 */
rss_array_result* rss_array_add(rss_array* array, const void* element, size_t element_size);

/**
 * @brief   Grow the capacity of the array to the new capacity.
 *          The new capacity must be strictly larger. Successful reallocation
 *          invalidates previously obtained pointers into the element buffer.
 *
 * @param   {rss_array*} array - the rss_array to update the capacity of
 * @param   {size_t} new_cap - the new capacity of the array
 *
 * @return  {rss_array_result* } - a pointer to the rss_array_result structure containing the result of the capacity grow operation
 */
rss_array_result* rss_array_capacity_grow(rss_array* array, size_t new_cap);

/**
 * @brief   Delete an element from the array at the given index.
 *
 * @param   {rss_array*} array - the rss_array to delete the element from
 * @param   {size_t} index - the index of the element to delete
 *
 * @return  {rss_array_result*} - operation status; invalid indices are errors
 */
rss_array_result* rss_array_delete(rss_array* array, size_t index);

/**
 * @brief   Does the array have space for more elements?
 *          Space is available if the length of the array
 *          is less than the capacity of the array.
 *
 * @param   {rss_array*} array - the rss_array to check without modifying it
 *
 * @return  {rss_array_result*} - status plus has_space; a full array is not an error
 */
rss_array_result* rss_array_has_space(rss_array* array);

/**
 * @brief   Reduce memory usage by compressing the capacity of the array
 *          to match the current length of the array.
 *          Empty arrays are rejected. Successful reallocation invalidates
 *          previously obtained pointers into the element buffer.
 *
 * @param   {rss_array*} array - the rss_array to compress the capacity of
 *
 * @return  {rss_array_result*} - operation status
 */
rss_array_result* rss_array_compress(rss_array* array);

#endif //ROMANO_SCHOONHEIM_C_STRINGS_ARRAY_H
