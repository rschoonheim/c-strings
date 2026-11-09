# C - strings

Yet another C strings library — a small, static C23 library providing a
length-aware string type, alphabet validation, and string iteration.

## Features

- **`rss_string`** — length + capacity tracked string with explicit null terminator
- **`rss_alphabet`** — define a character set and validate strings against it
- **`rss_string_iterator`** — safe, bounds-checked iteration over a string
- **Secure wipe** — `rss_string_wipe()` zeroes string contents before freeing, preventing sensitive data recovery from memory
- Explicit error reporting via result structs and error codes (no `errno`)

## Requirements

- C23-capable compiler (uses `bool` / `nullptr`)
- CMake ≥ 4.3

## Building

```bash
cmake -B build
cmake --build build
```

This produces the static library `libromano_schoonheim_c_strings.a`.

## Running tests

```bash
ctest --test-dir build
```

## Usage

```c
#include "includes/string.h"
#include "includes/alphabet.h"
#include "includes/iterator.h"

int main(void)
{
    // Create a string (len = 5, cap = 10)
    rss_create_result* result = rss("hello", 5, 10);
    if (result->error) {
        return result->error_code;
    }
    rss_string* s = result->string;

    // Query it
    rss_string_contains_char(s, 'e');              // true
    rss_string_contains_substring(s, "ell");       // true
    rss_string_length_between(s, 1, 10);           // true

    // Validate against an alphabet
    rss_alphabet* lower = rss_alphabet_create("abcdefghijklmnopqrstuvwxyz", 26);
    rss_alphabet_accepts(lower, s);                // true

    // Iterate
    rss_iterator_create_result* it_result = rss_iterator_create(s);
    rss_string_iterator* it = it_result->iterator;
    while (!rss_iterator_at_end(it)) {
        char c = rss_iterator_current_char(it);
        rss_iterator_index_move(it, 1);
    }

    // Clean up — secure=true also wipes and frees the underlying string
    rss_iterator_destroy(it, true);
    return 0;
}
```

## API overview

### `string.h`
| Function | Description |
|---|---|
| `rss(str, len, cap)` | Create an `rss_string`; returns an `rss_create_result` with error info |
| `rss_string_contains_char(s, c)` | Check if the string contains a character |
| `rss_string_contains_substring(s, sub)` | Check if the string contains a substring |
| `rss_string_length_between(s, min, max)` | Check if the string length is within bounds |
| `rss_string_wipe(s)` | Securely zero the string contents, then free it |

### `alphabet.h`
| Function | Description |
|---|---|
| `rss_alphabet_create(chars, len)` | Create an alphabet from a character set |
| `rss_alphabet_accepts(alphabet, s)` | True if the string only contains characters from the alphabet |

### `iterator.h`
| Function | Description |
|---|---|
| `rss_iterator_create(string)` | Create an iterator for a string |
| `rss_iterator_at_end(it)` | True if the iterator reached the end |
| `rss_iterator_index_move(it, amount)` | Advance the iterator by `amount` |
| `rss_iterator_current_char(it)` | Current character, or `'\0'` at the end |
| `rss_iterator_destroy(it, secure)` | Free the iterator; if `secure`, also wipe and free the string |

## Error codes

| Constant | Meaning |
|---|---|
| `RSS_STRING_ERROR_MEMORY_ALLOCATION_FAILED` | `malloc` failed |
| `RSS_STRING_ERROR_STR_LEN_MISMATCH` | Given length doesn't match the input string |
| `RSS_STRING_ERROR_LENGTH_GREATER_THAN_CAPACITY` | `len > cap` |
| `RSS_STRING_ITERATOR_ERROR_STRING_NULL` | Iterator created with a null string |
| `RSS_STRING_ITERATOR_ERROR_MEMORY_ALLOCATION_FAILED` | `malloc` failed |

## Project layout

```
includes/   Public headers (string.h, alphabet.h, iterator.h)
src/        Implementations
test/       CTest-driven test executables
```