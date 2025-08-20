#include "unity.h"
#include "../src/parse.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void test_cmd_enum_f1(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "test test";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = FAIL;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

void test_cmd_enum_f2(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = FAIL;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

void test_cmd_enum_f3(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "\t";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = FAIL;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_cmd_enum_f1);
    RUN_TEST(test_cmd_enum_f2);
    RUN_TEST(test_cmd_enum_f3);

    return UNITY_END();
}
