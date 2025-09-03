#!usr/bin/bash

UNITY_URL=https://raw.githubusercontent.com/ThrowTheSwitch/Unity/refs/heads/master/src/

# Download Unity test files if unity dir doesn't exist
if [ ! -d test/unity ]; then
    wget -P test/unity/ $UNITY_URL/unity.c
    wget -P test/unity/ $UNITY_URL/unity.h
    wget -P test/unity/ $UNITY_URL/unity_internals.h
fi

# Compile unity.c
gcc -o test/unity/unity -c test/unity/unity.c

mkdir -p test/build/

# Run string tests
gcc -o test/build/test_stringutil test/test_stringutil.c test/unity/unity \
                                        src/stringutil.c
echo "---------- String Tests -----------" > test/test_output.txt
./test/build/test_stringutil >> test/test_output.txt

# Run parse tests
gcc -o test/build/test_parse test/test_parse.c test/unity/unity src/parse.c src/stringutil.c
echo "---------- Parse Tests ------------" >> test/test_output.txt
./test/build/test_parse >> test/test_output.txt

# Run hashtable tests
gcc -o test/build/test_hashtable test/test_hashtable.c test/unity/unity src/hashtable.c \
                                                            src/stringutil.c
echo "--------- Hashtable Tests ---------" >> test/test_output.txt
./test/build/test_hashtable >> test/test_output.txt

# Hashtable memory allocation/deallocation test
gcc -o test/build/test_mem_hashtable test/test_mem_hashtable.c src/hashtable.c src/stringutil.c
echo "---------- Hashtable Memory Test ------------" >> test/test_output.txt
valgrind ./test/build/test_mem_hashtable 2>> test/test_output.txt

# Clean test directory
rm -rf test/build/
