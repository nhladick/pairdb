#include <stddef.h>
#include <stdbool.h>

#include "unity.h"
#include "../src/hashtable.h"
#include "../src/keydbstring.h"

void setUp(void)
{

}

void tearDown(void)
{

}


void test_init_and_size(void)
{
    hashtbl tbl = init_hashtbl(4);
    TEST_ASSERT_NOT_NULL(tbl);
    size_t size = get_tbl_size(tbl);
    TEST_ASSERT_EQUAL_INT(4, size);
    size_t numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(0, numentries);

    destroy_hashtbl(tbl);
}

void test_put_and_size(void)
{
    hashtbl tbl = init_hashtbl(8);
    size_t numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(0, numentries);

    int result = put(tbl, "key1", "val1");
    numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(1, numentries);

    result = put(tbl, "key2", "val2");
    numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(2, numentries);

    result = put(tbl, "key1", "val3");
    numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(-1, result);
    TEST_ASSERT_EQUAL_INT(2, numentries);

    destroy_hashtbl(tbl);
}

void test_put_and_delete_size(void)
{
    hashtbl tbl = init_hashtbl(8);
    put(tbl, "key1", "val1");
    put(tbl, "key2", "val2");
    put(tbl, "key3", "val3");
    size_t numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(3, numentries);

    delete(tbl, "key1");
    numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(2, numentries);

    delete(tbl, "key1");
    numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(2, numentries);

    delete(tbl, "nokey");
    numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(2, numentries);

    destroy_hashtbl(tbl);
}

void test_resize(void)
{
    hashtbl tbl = init_hashtbl(2);
    size_t size = get_tbl_size(tbl);
    TEST_ASSERT_EQUAL_INT(2, size);

    put(tbl, "key1", "val1");
    put(tbl, "key2", "val2");
    put(tbl, "key3", "val3");

    size = get_tbl_size(tbl);
    TEST_ASSERT_EQUAL_INT(4, size);

    put(tbl, "key4", "val4");

    size = get_tbl_size(tbl);
    TEST_ASSERT_EQUAL_INT(8, size);

    size_t numentries = get_numentries(tbl);
    TEST_ASSERT_EQUAL_INT(4, numentries);

    destroy_hashtbl(tbl);
}

void test_find(void)
{
    hashtbl tbl = init_hashtbl(4);
    put(tbl, "key1", "val1");

    char str1[6];
    size_t result = find(str1, 6, tbl, "key1");
    TEST_ASSERT_EQUAL_INT(4, result);
    TEST_ASSERT_EQUAL_STRING("val1", str1);

    char str2[6];
    result = find(str2, 6, tbl, "nokey");
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("", str2);

    destroy_hashtbl(tbl);
}

void test_exists(void)
{
    hashtbl tbl = init_hashtbl(4);
    put(tbl, "key1", "val1");
    bool result1 = exists(tbl, "key1");
    TEST_ASSERT_EQUAL_INT(true, result1);

    bool result2 = exists(tbl, "key2");
    TEST_ASSERT_EQUAL_INT(false, result2);

    destroy_hashtbl(tbl);
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_init_and_size);
    RUN_TEST(test_put_and_size);
    RUN_TEST(test_put_and_delete_size);
    RUN_TEST(test_resize);
    RUN_TEST(test_find);
    RUN_TEST(test_exists);

    return UNITY_END();
}
