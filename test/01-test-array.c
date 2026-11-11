#include "../src/array.c"

#include <assert.h>

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
}

static void test_rss_array_capacity_grow()
{
    // Test scenario 1: array nullptr
    //
    assert(rss_array_capacity_grow(nullptr, 20) == false);

    // Test scenario 2: new_cap exceeds SIZE_MAX / element_size
    //
    rss_array* array2 = rss_array_create(sizeof(int), 10);
    assert(array2 != nullptr);
    assert(rss_array_capacity_grow(array2, SIZE_MAX / sizeof(int) + 1) == false);
    free(array2->elements);
    free(array2);

    // Test scenario 3: new_cap is less than current length
    //
    rss_array* array3 = rss_array_create(sizeof(int), 10);
    assert(array3 != nullptr);
    int element3 = 42;
    assert(rss_array_add(array3, &element3, sizeof(int)) == true); // len = 1
    assert(rss_array_capacity_grow(array3, 0) == false); // new_cap < len
    free(array3->elements);
    free(array3);

    // Test scenario 4: successful capacity growth
    //
    rss_array* array4 = rss_array_create(sizeof(int), 10);
    assert(array4 != nullptr);
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

int main()
{
    test_rss_array_create();
    test_rss_array_destroy();
    test_rss_array_add();
    test_rss_array_capacity_grow();
    test_rss_array_delete();

    return 0;
}