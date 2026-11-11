#include "../src/array.c"

#include <assert.h>
#include <stdio.h>

static void test_array_maximum_size()
{
    // Test case 1: nullptr array
    //
    assert(array_maximum_size(0) == 0);

    // Test case 2: element_size is 0
    //
    rss_array array2 = { nullptr, 0, 0, 10 };
    assert(array_maximum_size(array2.element_size) == 0);

    // Test case 3: normal case
    //
    rss_array *array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    size_t expected_max_size = SIZE_MAX / sizeof(int);
    assert(array_maximum_size(array3->element_size) == expected_max_size);
}

static void test_rss_array_validate()
{
    // Test case 1: nullptr array
    //
    assert(rss_array_validate(nullptr) == false);

    // Test case 2: valid array
    //
    rss_array *array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    assert(rss_array_validate(array2) == true);
    free(array2->elements);
    free(array2);

    // Test case 3: invalid array (elements is nullptr)
    //
    rss_array array3 = { nullptr, sizeof(int), 0, 10 };
    assert(rss_array_validate(&array3) == false);

    // Test case 4: invalid array (len > cap)
    //
    rss_array array4 = { malloc(sizeof(int) * 10), sizeof(int), 11, 10 };
    assert(rss_array_validate(&array4) == false);
    free(array4.elements);

    // Test case 5: invalid array (element_size is 0)
    //
    rss_array array5 = { malloc(sizeof(int) * 10), 0, 0, 10 };
    assert(rss_array_validate(&array5) == false);
    free(array5.elements);
}

static void test_rss_array_create()
{
    // Test scenario 1: successful creation
    //
    rss_array* array1 = rss_array_create(sizeof(int), 10);
    assert(array1 != nullptr);
    assert(array1->element_size == sizeof(int));
    assert(array1->len == 0);
    assert(array1->cap == 10);
    free(array1->elements);
    free(array1);

    // Test scenario 2: element_size is 0
    //
    rss_array* array2 = rss_array_create(0, 10);
    assert(array2 == nullptr);

    // Test scenario 3: cap exceeds SIZE_MAX / element_size
    //
    rss_array* array3 = rss_array_create(sizeof(int), SIZE_MAX / sizeof(int) + 1);
    assert(array3 == nullptr);
}

static void test_rss_array_destroy()
{
    // Test scenario 1: successful destruction
    //
    rss_array* array1 = rss_array_create(sizeof(int), 10);
    assert(array1 != nullptr);
    assert(rss_array_destroy(array1) == true);

    // Test scenario 2: array is nullptr
    //
    assert(rss_array_destroy(nullptr) == false);
}

static void test_rss_array_add()
{
    // Test scenario 1: array nullptr
    //
    int element1 = 42;
    assert(rss_array_add(nullptr, &element1, sizeof(int)) == false);

    // Test scenario 2: element nullptr
    //
    rss_array* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    assert(rss_array_add(array2, nullptr, sizeof(int)) == false);
    free(array2->elements);
    free(array2);

    // Test scenario 3: adding element with different size
    //                  than array's element size
    //
    rss_array* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    double element3 = 3.14;
    assert(rss_array_add(array3, &element3, sizeof(double)) == false);
    free(array3->elements);
    free(array3);

    // Test scenario 4: appends element to the end of the array
    //
    rss_array* array4 = rss_array_create(sizeof(int), 10);
    assert(array4 != nullptr);
    int element4 = 42;
    assert(rss_array_add(array4, &element4, sizeof(int)) == true);
    assert(array4->len == 1);
    assert(((int*)array4->elements)[0] == 42);
    free(array4->elements);
    free(array4);

    // Test scenario 5: adding element with element size 0
    //                  results in failure.
    //
    rss_array* array5 = rss_array_create(sizeof(int), 10);
    assert(array5 != nullptr);
    int element5 = 42;
    assert(rss_array_add(array5, &element5, 0) == false);
    free(array5->elements);
    free(array5);

    // Test scenario 6: adding element when array is full
    //
    rss_array* array6 = rss_array_create(sizeof(int), 1);
    assert(array6 != nullptr);
    int element6a = 42;
    int element6b = 43;
    assert(rss_array_add(array6, &element6a, sizeof(int)) == true);
    assert(rss_array_add(array6, &element6b, sizeof(int)) == false); // array is full
    free(array6->elements);
    free(array6);
}

static void test_rss_array_capacity_grow()
{
    // Test scenario 1: array nullptr
    //
    assert(rss_array_capacity_grow(nullptr, 20) == false);

    // Test scenario 2: new capacity exceeds maximum size
    //
    rss_array* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    assert(rss_array_capacity_grow(array2, SIZE_MAX / sizeof(int) + 1) == false);
    free(array2);

    // Test scenario 3: new capacity less than current length
    //
    rss_array* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    int element3 = 42;
    assert(rss_array_add(array3, &element3, sizeof(int)) == true);
    assert(rss_array_capacity_grow(array3, 0) == false); // new
    free(array3->elements);
    free(array3);

    // Test scenario 4: successful capacity grow
    //
    rss_array* array4 = rss_array_create(sizeof(int), 10);
    assert(array4 != nullptr);
    int element4 = 42;
    assert(rss_array_add(array4, &element4, sizeof(int)) == true);
    assert(rss_array_capacity_grow(array4, 20) == true);
    assert(array4->cap == 20);
    free(array4->elements);
    free(array4);
}

static void test_rss_array_delete()
{
    // Test scenario 1: array nullptr
    //
    assert(rss_array_delete(nullptr, 0) == false);

    // Test scenario 2: index out of bounds
    //
    rss_array* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    assert(rss_array_delete(array2, 0) == false); // len = 0, index = 0 is out of bounds
    free(array2->elements);
    free(array2);

    // Test scenario 3: successful deletion
    //
    rss_array* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    int element3a = 42;
    int element3b = 43;
    assert(rss_array_add(array3, &element3a, sizeof(int)) == true); // len = 1
    assert(rss_array_add(array3, &element3b, sizeof(int)) == true); // len = 2
    assert(rss_array_delete(array3, 0) == true); // delete first element
    assert(array3->len == 1);
    assert(((int*)array3->elements)[0] == 43); // second element shifted to first position
    free(array3->elements);
    free(array3);
}

static void test_rss_array_has_space()
{
    // Test scenario 1: array nullptr
    //
    assert(rss_array_has_space(nullptr) == false);

    // Test scenario 2: array has space
    //
    rss_array* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    assert(rss_array_has_space(array2) == true);
    free(array2->elements);
    free(array2);

    // Test scenario 3: array is full
    //
    rss_array* array3 = rss_array_create(sizeof(int), 1);
    assert(array3 != nullptr);
    int element3 = 42;
    assert(rss_array_add(array3, &element3, sizeof(int)) == true); // len = 1, cap = 1
    assert(rss_array_has_space(array3) == false); // no space left
    free(array3->elements);
    free(array3);
}

static void test_rss_array_compress()
{
    // Test scenario 1: array nullptr
    //
    assert(rss_array_compress(nullptr) == false);

    // Test scenario 2: successful capacity compress
    //
    rss_array* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);

    // Add some elements to the array
    int element2a = 42;
    int element2b = 43;
    assert(rss_array_add(array2, &element2a, sizeof(int)) == true);
    assert(rss_array_add(array2, &element2b, sizeof(int)) == true);
    assert(array2->len == 2);

    assert(rss_array_compress(array2) == true);
    assert(array2->cap == array2->len); // capacity should now equal length
    free(array2->elements);
    free(array2);

    // Test scenario 3: compressing an empty array
    //
    rss_array* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    assert(rss_array_compress(array3) == false);
}

int main()
{
    test_array_maximum_size();
    test_rss_array_validate();
    test_rss_array_create();
    test_rss_array_destroy();
    test_rss_array_add();
    test_rss_array_capacity_grow();
    test_rss_array_delete();
    test_rss_array_has_space();
    test_rss_array_compress();

    return 0;
}