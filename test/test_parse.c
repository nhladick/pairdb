#include "unity.h"
#include "../src/parse.h"

void setUp(void)
{

}

void tearDown(void)
{

}


// Newline char '\n' is included in all input strings
// because fgets leaves it in the buffer passed to
// the parse_input function


// Test failure on bad input
void test_cmd_enum_f1(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "test test\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = FAIL;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

// Test failure on bad input
void test_cmd_enum_f2(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = FAIL;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

// Test failure on bad input
void test_cmd_enum_f3(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "\t\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = FAIL;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

// Test newtbl command - enum value and table name str
void test_cmd_enum_and_str_nt(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "newtbl table1\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = NEWTABLE;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
    TEST_ASSERT_EQUAL_STRING("table1", parse_data.tbl_name);
}

// Test use command - enum value and table name str
void test_cmd_enum_and_str_ut(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "use table1\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = USETABLE;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
    TEST_ASSERT_EQUAL_STRING("table1", parse_data.tbl_name);
}

// Test add command - enum value and key and val strings
void test_cmd_enum_and_str_add(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "add key1 val1\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = ADD;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
    TEST_ASSERT_EQUAL_STRING("key1", parse_data.key);
    TEST_ASSERT_EQUAL_STRING("val1", parse_data.val);
}

// Test get command - enum value and key string
void test_cmd_enum_and_str_get(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "get key1\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = GET;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
    TEST_ASSERT_EQUAL_STRING("key1", parse_data.key);
}

// Test del command - enum value and key str
void test_cmd_enum_and_str_del(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "del key1\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = DELETE;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
    TEST_ASSERT_EQUAL_STRING("key1", parse_data.key);
}

// Test save command enum value
void test_cmd_enum_save(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "save\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = SAVE;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

// Test quit command enum value
void test_cmd_enum_quit(void)
{
    struct parse_object parse_data = {0};
    char inbuff[] = "quit\n";
    parse_input(inbuff, &parse_data);
    enum CMD cmd = QUIT;
    TEST_ASSERT_EQUAL_INT(cmd, parse_data.cmd);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_cmd_enum_f1);
    RUN_TEST(test_cmd_enum_f2);
    RUN_TEST(test_cmd_enum_f3);
    RUN_TEST(test_cmd_enum_and_str_nt);
    RUN_TEST(test_cmd_enum_and_str_ut);
    RUN_TEST(test_cmd_enum_and_str_add);
    RUN_TEST(test_cmd_enum_and_str_get);
    RUN_TEST(test_cmd_enum_and_str_del);
    RUN_TEST(test_cmd_enum_save);
    RUN_TEST(test_cmd_enum_quit);

    return UNITY_END();
}
