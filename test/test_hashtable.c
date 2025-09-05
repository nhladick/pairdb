#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "unity/unity.h"
#include "../src/hashtable.h"
#include "../src/stringutil.h"

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

    char str1[6] = {0};
    size_t result = find(str1, 6, tbl, "key1");
    TEST_ASSERT_EQUAL_INT(4, result);
    TEST_ASSERT_EQUAL_STRING("val1", str1);

    char str2[6] = {0};
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

void test_hashtbl_fileio(void)
{
    hashtbl tbl = init_hashtbl(8);

    put(tbl, "key1", "val1");
    put(tbl, "key2", "val2");
    put(tbl, "key3", "val3");

    size_t initsize = get_tbl_size(tbl);
    size_t initnumentries = get_numentries(tbl);

    char buff[256] = {0};

    FILE *outf = fmemopen(buff, sizeof(buff), "w");
    hashtbl_to_file(tbl, outf);
    fclose(outf);

    destroy_hashtbl(tbl);

    FILE *inf = fmemopen(buff, sizeof(buff), "r");
    hashtbl tbl2 = load_hashtbl_from_file(inf);

    size_t postsize = get_tbl_size(tbl2);
    size_t postnumentries = get_numentries(tbl2);

    TEST_ASSERT_EQUAL_INT(initsize, postsize);
    TEST_ASSERT_EQUAL_INT(initnumentries, postnumentries);

    bool k1exist = exists(tbl2, "key1");
    bool k2exist = exists(tbl2, "key2");
    bool k3exist = exists(tbl2, "key3");

    TEST_ASSERT_EQUAL_INT(true, k1exist);
    TEST_ASSERT_EQUAL_INT(true, k2exist);
    TEST_ASSERT_EQUAL_INT(true, k3exist);

    char valbuff[HT_VAL_MAX];

    find(valbuff, HT_VAL_MAX, tbl2, "key1");
    TEST_ASSERT_EQUAL_STRING("val1", valbuff);
    find(valbuff, HT_VAL_MAX, tbl2, "key2");
    TEST_ASSERT_EQUAL_STRING("val2", valbuff);
    find(valbuff, HT_VAL_MAX, tbl2, "key3");
    TEST_ASSERT_EQUAL_STRING("val3", valbuff);

    fclose(inf);
    destroy_hashtbl(tbl2);
}

void test_get_keys(void)
{
    hashtbl tbl = init_hashtbl(4);
    put(tbl, "key1", "val1");
    put(tbl, "key2", "val2");

    char **keys = get_keys(tbl);

    bool k1 = (strcmp(keys[0], "key1") == 0 ||
               strcmp(keys[1], "key1") == 0);

    bool k2 = (strcmp(keys[0], "key2") == 0 ||
               strcmp(keys[1], "key2") == 0);

    TEST_ASSERT_EQUAL_INT(true, k1);
    TEST_ASSERT_EQUAL_INT(true, k2);

    free(keys);
    destroy_hashtbl(tbl);
}

void test_get_vals(void)
{
    hashtbl tbl = init_hashtbl(4);
    put(tbl, "key1", "val1");
    put(tbl, "key2", "val2");

    char **vals = get_vals(tbl);

    bool v1 = (strcmp(vals[0], "val1") == 0 ||
               strcmp(vals[1], "val1") == 0);

    bool v2 = (strcmp(vals[0], "val2") == 0 ||
               strcmp(vals[1], "val2") == 0);

    TEST_ASSERT_EQUAL_INT(true, v1);
    TEST_ASSERT_EQUAL_INT(true, v2);

    free(vals);
    destroy_hashtbl(tbl);
}

/*------- Tests to check behavior on uninitialized input ------*/

void test_null_destroy(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    destroy_hashtbl(tbl);
    TEST_ASSERT_EQUAL_INT(0, tbl);
}

void test_null_get_size(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    size_t result = get_tbl_size(tbl);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_null_get_numentries(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    size_t result = get_tbl_size(tbl);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_null_put(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    size_t result = put(tbl, "key1", "val1");
    TEST_ASSERT_EQUAL_INT(-2, result);
}

void test_null_find(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    char buff[6] = {0};
    size_t result = find(buff, 6, tbl, "key1");
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_null_exists(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    bool result = exists(tbl, "key1");
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_null_delete(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    delete(tbl, "key1");
    TEST_ASSERT_EQUAL_INT(0, tbl);
}

void test_null_get_keys(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    char **k = get_keys(tbl);
    TEST_ASSERT_EQUAL_INT(0, k);
}

void test_null_get_vals(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);
    char **v = get_vals(tbl);
    TEST_ASSERT_EQUAL_INT(0, v);
}

void test_null_tofile(void)
{
    hashtbl tbl = NULL;
    TEST_ASSERT_EQUAL_INT(0, tbl);

    // Valid file, invalid hashtbl
    char buff[256] = {0};

    FILE *outf = fmemopen(buff, sizeof(buff), "w");
    size_t result = hashtbl_to_file(tbl, outf);
    fclose(outf);
    TEST_ASSERT_EQUAL_INT(0, result);

    // Valid hashtbl, invalid file
    tbl = init_hashtbl(4);
    outf = NULL;
    result = hashtbl_to_file(tbl, outf);
    TEST_ASSERT_EQUAL_INT(0, result);

    destroy_hashtbl(tbl);

    // Invalid hashtbl and invalid file
    tbl = NULL;
    outf = NULL;
    result = hashtbl_to_file(tbl, outf);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_null_fromfile(void)
{
    FILE *f = NULL;
    hashtbl tbl = load_hashtbl_from_file(f);
    TEST_ASSERT_EQUAL_INT(0, tbl);
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
    RUN_TEST(test_hashtbl_fileio);
    RUN_TEST(test_get_keys);
    RUN_TEST(test_get_vals);
    RUN_TEST(test_null_destroy);
    RUN_TEST(test_null_get_size);
    RUN_TEST(test_null_get_numentries);
    RUN_TEST(test_null_put);
    RUN_TEST(test_null_find);
    RUN_TEST(test_null_exists);
    RUN_TEST(test_null_delete);
    RUN_TEST(test_null_get_keys);
    RUN_TEST(test_null_get_vals);
    RUN_TEST(test_null_tofile);
    RUN_TEST(test_null_fromfile);

    return UNITY_END();
}
