#!usr/bin/bash

# Perform tests for pairdb application
# Uses Unity testing framework from ThrowTheSwitch
# https://github.com/ThrowTheSwitch/Unity

# Testing source files
STRUTIL_TEST=test/test_stringutil.c
PARSE_TEST=test/test_parse.c
HTABLE_TEST=test/test_hashtable.c
MEM_TEST=test/test_mem_hashtable.c

# All test output written to output file
TEST_OUT=test/test_output.txt

# URL to download unity test files
# Unity project covered under MIT License
# https://github.com/ThrowTheSwitch/Unity?tab=MIT-1-ov-file
UNITY_URL=https://raw.githubusercontent.com/ThrowTheSwitch/Unity/refs/heads/master/src/

# Download Unity test files if unity dir doesn't exist
if [ ! -d test/unity ]; then
    wget -P test/unity/ $UNITY_URL/unity.c
    wget -P test/unity/ $UNITY_URL/unity.h
    wget -P test/unity/ $UNITY_URL/unity_internals.h
fi

# Compile unity.c
gcc -o test/unity/unity -c test/unity/unity.c
UNITY_OBJ=test/unity/unity

# Temporary test build directory
mkdir -p test/build/

# If object files being tested already exist in main build directory,
# use those object files. Else, compile object files in test/build directory

# stringutil
STRUTIL_OBJ=""
if [ -f build/stringutil.o ]; then
    STRUTIL_OBJ=build/stringutil.o
else
    gcc -o test/build/stringutil.o -c src/stringutil.c
    STRUTIL_OBJ=test/build/stringutil.o
fi

# parse
PARSE_OBJ=""
if [ -f build/parse.o ]; then
    PARSE_OBJ=build/parse.o
else
    gcc -o test/build/parse.o -c src/parse.c
    PARSE_OBJ=test/build/parse.o
fi

#hashtable
HTABLE_OBJ=""
if [ -f build/hashtable.o ]; then
    HTABLE_OBJ=build/hashtable.o
else
    gcc -o test/build/hashtable.o -c src/hashtable.c
    HTABLE_OBJ=test/build/hashtable.o
fi

# Build and run string tests
gcc -o test/build/test_stringutil $STRUTIL_TEST $UNITY_OBJ $STRUTIL_OBJ
echo "---------- String Tests -----------" > $TEST_OUT
./test/build/test_stringutil >> $TEST_OUT

# Build and run parse tests
gcc -o test/build/test_parse $PARSE_TEST $UNITY_OBJ $PARSE_OBJ $STRUTIL_OBJ
echo "---------- Parse Tests ------------" >> $TEST_OUT
./test/build/test_parse >> $TEST_OUT

# Build and run hashtable tests
gcc -o test/build/test_hashtable $HTABLE_TEST $UNITY_OBJ $HTABLE_OBJ $STRUTIL_OBJ
echo "--------- Hashtable Tests ---------" >> $TEST_OUT
./test/build/test_hashtable >> $TEST_OUT

# Build and run hashtable memory allocation/deallocation test
gcc -o test/build/test_mem_hashtable $MEM_TEST $HTABLE_OBJ $STRUTIL_OBJ
echo "---------- Hashtable Memory Test ------------" >> $TEST_OUT
valgrind ./test/build/test_mem_hashtable 2>> $TEST_OUT

# Clean test directory
rm -rf test/build/
