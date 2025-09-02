#include <string.h>
#include <stddef.h>

#include "unity/unity.h"
#include "../src/pairdbstring.h"

void setUp(void)
{

}

void tearDown(void)
{

}


void test_strtcpy(void)
{
    char *str = "test string 1";
    size_t len = strlen(str);
    char buff[15];
    ssize_t result = strtcpy(buff, str, 15);
    TEST_ASSERT_EQUAL_INT(len, result);
    TEST_ASSERT_EQUAL_STRING("test string 1", buff);
}

void test_strtcpy_short(void)
{
    char *str = "test string 1";
    char buff[8];
    ssize_t result = strtcpy(buff, str, 4);
    TEST_ASSERT_EQUAL_INT(-1, result);
    TEST_ASSERT_EQUAL_STRING("tes", buff);
}

void test_getrandstr(void)
{
    char buff[11];
    getrandstr(buff, 11);
    TEST_ASSERT_EQUAL_INT(10, strlen(buff));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_strtcpy);
    RUN_TEST(test_strtcpy_short);
    RUN_TEST(test_getrandstr);

    return UNITY_END();
}
