# C - strings

Yet another C strings library — a small, static C23 library providing
generic arrays, length-aware strings, alphabet validation, and string iteration.

## Features

- **`rss_array`** — generic array with explicit element size, length, and capacity
- **`rss_string`** — length- and capacity-tracked string with an explicit null terminator
- **`rss_alphabet`** — define a character set and validate strings against it
- **`rss_string_iterator`** — bounds-checked iteration over a string
- **String wiping** — `rss_wipe()` wipes string contents before freeing
- Explicit error reporting through result structures and error codes (no `errno`)

## Requirements

- C23-capable compiler supporting `bool` and `nullptr`
- CMake ≥ 4.3

## Building

```bash
cmake -B build
cmake --build build
```

This produces the static library `libromano_schoonheim_c_strings.a`.

## Running tests

```bash
ctest --test-dir build --output-on-failure
```

Run only the array tests:

```bash
ctest --test-dir build -R '^01_test_array$' --output-on-failure
```

## Array usage

All array operations return an `rss_array_result*`, including validation,
destruction, and space queries.

This example exits on any error. Applications that need recovery should
handle errors and preserve their array handles instead.

```c
#include "includes/array.h"

#include <stdio.h>
#include <stdlib.h>

static rss_array_result* require_success(rss_array_result* result)
{
    if (result == nullptr)
    {
        fputs("Could not allocate an array result\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (result->error)
    {
        fprintf(stderr, "Array error: %d\n", result->error_code);
        free(result);
        exit(EXIT_FAILURE);
    }

    return result;
}

int main(void)
{
    // Create an empty array with room for four integers.
    rss_array_result* result =
        require_success(rss_array_create(sizeof(int), 4));

    rss_array* array = result->array;
    free(result); // Frees the result wrapper, not the array.

    // Append a copy of an integer.
    int value = 42;
    result = require_success(rss_array_add(array, &value, sizeof(value)));
    free(result);

    // A full array is not an error: inspect has_space separately.
    result = require_success(rss_array_has_space(array));
    printf("Has space: %s\n", result->has_space ? "yes" : "no");
    free(result);

    // Capacity changes are explicit; adding does not automatically grow.
    result = require_success(rss_array_capacity_grow(array, 8));
    free(result);

    // Read initialized elements using the correct element type.
    printf("First element: %d\n", ((int*)array->elements)[0]);

    // Shrink capacity to the current length, which is one.
    result = require_success(rss_array_compress(array));
    free(result);

    // Remove the element. The array remains valid but empty.
    result = require_success(rss_array_delete(array, 0));
    free(result);

    // Free both the element buffer and the array structure.
    result = require_success(rss_array_destroy(array));
    array = nullptr;
    free(result);

    return EXIT_SUCCESS;
}
```

## Array results and ownership

`rss_array_result` contains:

| Field | Meaning |
|---|---|
| `error` | `true` when the operation failed |
| `error_code` | Error constant; `RSS_ARRAY_ERROR_NONE` on success |
| `array` | Array handle on success; `nullptr` on error or after successful destruction |
| `has_space` | Boolean query value for `rss_array_has_space()` |

### Handling results

1. Check whether the result pointer is `nullptr` before dereferencing it.
2. Check `result->error` before using the success payload.
3. For space queries, read `result->has_space` only after checking for errors.
4. Release every non-null result with `free()`.

A `nullptr` return means allocation of the result itself failed. Other
failures return a result with `error == true`. In either case, the operation
leaves an existing array unchanged.

### Ownership rules

- The caller owns an array returned by successful creation.
- Freeing a result does **not** destroy its array.
- Results from subsequent operations refer to the same array; they do not
  create additional ownership.
- Destroy each owned array exactly once with `rss_array_destroy()`.
- Destruction also returns a result and can fail to allocate it. Do not discard
  the array handle unless destruction succeeds.
- Destruction frees the element buffer, but does not follow or free pointers
  stored as elements.
- Successful growth or compression invalidates previously obtained pointers
  into the element buffer. Obtain new pointers from `array->elements`.
- Array handles must remain valid, and concurrent access must be synchronized.

### Array constraints

- Creation requires nonzero `element_size` and capacity.
- Adding requires at least `element_size` readable source bytes and an exact
  match with the array's element size.
- Adding to a full array returns an error; growth is explicit.
- Growth requires a capacity strictly greater than the current capacity.
- Deletion requires an index below `len` and shifts subsequent elements down.
- Compression requires a nonempty array and sets capacity to length.
- Only elements below `len` are initialized and available for reading.
- Validation checks metadata, not pointer lifetime, ownership, or the actual
  allocated buffer size. It cannot make an arbitrary pointer safe to use.

## API overview

### `array.h`

Every function returns `rss_array_result*`.

| Function | Description |
|---|---|
| `rss_array_create(element_size, cap)` | Create an empty array with space for `cap` elements |
| `rss_array_validate(array)` | Check array metadata without modifying the array |
| `rss_array_destroy(array)` | Free the element buffer and array structure |
| `rss_array_add(array, element, element_size)` | Append a copy of an element if capacity permits |
| `rss_array_capacity_grow(array, new_cap)` | Increase capacity while preserving existing elements |
| `rss_array_delete(array, index)` | Remove an element and shift subsequent elements down |
| `rss_array_has_space(array)` | Report whether `len < cap` through `result->has_space` |
| `rss_array_compress(array)` | Reduce a nonempty array's capacity to its length |

### `string.h`

| Function | Description |
|---|---|
| `rss(str, len, cap)` | Create an `rss_string`; returns an `rss_create_result` with error information |
| `rss_contains_char(s, c)` | Check whether the string contains a character |
| `rss_contains_substring(s, sub)` | Check whether the string contains a substring |
| `rss_length_between(s, min, max)` | Check whether the string length is within bounds |
| `rss_wipe(s)` | Wipe string contents, then free the string |

### `alphabet.h`

| Function | Description |
|---|---|
| `rss_alphabet_create(chars, len)` | Create an alphabet; returns an `rss_alphabet_create_result` |
| `rss_alphabet_destroy(alphabet)` | Free the alphabet and its character array |
| `rss_alphabet_accepts(alphabet, s)` | Check whether the string contains only characters from the alphabet |

### `iterator.h`

| Function | Description |
|---|---|
| `rss_iterator_create(string)` | Create an iterator for a string |
| `rss_iterator_at_end(it)` | Check whether the iterator reached the end |
| `rss_iterator_index_move(it, amount)` | Advance the iterator by `amount` |
| `rss_iterator_current_char(it)` | Return the current character, or `'\0'` at the end |
| `rss_iterator_destroy(it, secure)` | Free the iterator; if `secure`, also wipe and free the string |

## Error codes

### Arrays

These codes appear in `rss_array_result.error_code`. Failure to allocate
the result itself is reported by returning `nullptr`, not an error code.

| Constant | Value | Meaning |
|---|---:|---|
| `RSS_ARRAY_ERROR_NONE` | 0 | Operation succeeded |
| `RSS_ARRAY_ERROR_ARRAY_NULLPTR` | 1 | Array pointer is null |
| `RSS_ARRAY_ERROR_INVALID_STATE` | 2 | Invalid array state or invalid creation parameters |
| `RSS_ARRAY_ERROR_CAPACITY_EXCEEDS_MAX_SIZE` | 3 | Capacity exceeds the representable allocation size for the element size |
| `RSS_ARRAY_ERROR_MEMORY_ALLOCATION_FAILED` | 4 | Array storage allocation or reallocation failed |
| `RSS_ARRAY_ERROR_ELEMENT_NULLPTR` | 5 | Source element pointer is null |
| `RSS_ARRAY_ERROR_ELEMENT_SIZE_MISMATCH` | 6 | Source element size does not match the array's element size |
| `RSS_ARRAY_ERROR_LEN_GREATER_OR_EQUAL_TO_CAP` | 7 | No capacity remains for another element |
| `RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_TOO_SMALL` | 8 | Requested capacity is not strictly greater than current capacity |
| `RSS_ARRAY_ERROR_CAPACITY_GROW_FAILED_NEW_CAPACITY_EXCEEDS_MAX_SIZE` | 9 | Requested growth exceeds the maximum allocation size |
| `RSS_ARRAY_ERROR_INDEX_OUT_OF_BOUNDS` | 10 | Deletion index is outside the current length |
| `RSS_ARRAY_ERROR_ARRAY_EMPTY` | 11 | Cannot compress an empty array |

### Strings, alphabets, and iterators

| Constant | Meaning |
|---|---|
| `RSS_STRING_ERROR_MEMORY_ALLOCATION_FAILED` | `malloc` failed |
| `RSS_STRING_ERROR_STR_LEN_MISMATCH` | Given length does not match the input string |
| `RSS_STRING_ERROR_LENGTH_GREATER_THAN_CAPACITY` | `len > cap` |
| `RSS_ALPHABET_CREATION_ERROR_CHARACTERS_NULL` | `characters` was null |
| `RSS_ALPHABET_CREATION_ERROR_LEN_SIZE_MAX` | `len` equals `SIZE_MAX` and would overflow |
| `RSS_ALPHABET_CREATION_ERROR_MEMORY_ALLOCATION_FAILED` | `malloc` failed |
| `RSS_STRING_ITERATOR_ERROR_STRING_NULL` | Iterator created with a null string |
| `RSS_STRING_ITERATOR_ERROR_MEMORY_ALLOCATION_FAILED` | `malloc` failed |

## Project layout

```text
includes/   Public headers (array.h, string.h, alphabet.h, iterator.h)
src/        Implementations
test/       CTest-driven test executables
```