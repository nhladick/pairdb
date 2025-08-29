CC = gcc
CFLAGS = -Wall

SRC = src/
BUILD = build/
TEST = test

OBJ = $(BUILD)main.o \
	  $(BUILD)parse.o \
	  $(BUILD)fileio.o \
	  $(BUILD)memorydb.o \
	  $(BUILD)hashtable.o \
	  $(BUILD)keydbstring.o \
	  $(BUILD)db_manager.o

TEST_FILES = unity.c unity.h unity_internals.h
UNITY_URL = https://raw.githubusercontent.com/ThrowTheSwitch/Unity/refs/heads/master/src/

TARGET = $(BUILD)keydb

.PHONY: all clean init-test test

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(BUILD)%.o: $(SRC)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)main.o: $(SRC)parse.h $(SRC)memorydb.h $(SRC)db_manager.h
$(BUILD)parse.o: $(SRC)parse.h $(SRC)keydbstring.h
$(BUILD)fileio.o: $(SRC)fileio.h $(SRC)memorydb.h $(SRC)hashtable.h
$(BUILD)memorydb.o: $(SRC)memorydb.h $(SRC)hashtable.h $(SRC)keydbstring.h
$(BUILD)hashtable.o: $(SRC)hashtable.h $(SRC)keydbstring.h
$(BUILD)keydbstring.o: $(SRC)keydbstring.h
$(BUILD)db_manager.o: $(SRC)db_manager.h $(SRC)hashtable.h $(SRC)memorydb.h $(SRC)fileio.h \
			$(SRC)keydbstring.h

clean:
	rm -rf $(BUILD)

init-test:
	for file in $(TEST_FILES); do \
		wget -P $(TEST) $(UNITY_URL)$$file; \
	done

test:
	# Runs all tests with output to test/test_output.txt
	# Prints output file to stdout upon completion
	# If a test fails, output is not printed to stdout
	# (make notifies that a program had error output and
	# and doesn't continue)
	# Check test/test_output.txt for results

	# parse tests
	$(CC) -o test/test_parse test/test_parse.c test/unity.c src/parse.c src/keydbstring.c
	echo "---------- Parse Tests ------------" > test/test_output.txt
	./test/test_parse >> test/test_output.txt

	# hashtable tests
	$(CC) -o test/test_hashtable test/test_hashtable.c test/unity.c src/hashtable.c \
	src/keydbstring.c
	echo "---------- Hashtable Tests ------------" >> test/test_output.txt
	./test/test_hashtable >> test/test_output.txt

	# hashtable memory allocation/deallocation test
	$(CC) -o test/test_mem_hashtable test/test_mem_hashtable.c src/hashtable.c src/keydbstring.c
	echo "---------- Hashtable Memory Test ------------" >> test/test_output.txt
	valgrind ./test/test_mem_hashtable 2>> test/test_output.txt

	# print to stdout
	cat test/test_output.txt
