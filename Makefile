CC = gcc
CFLAGS = -Wall

SRC = src/
BUILD = build/

OBJ = $(BUILD)main.o \
	  $(BUILD)parse.o \
	  $(BUILD)fileio.o \
	  $(BUILD)memorydb.o \
	  $(BUILD)hashtable.o \
	  $(BUILD)keydbstring.o

TARGET = $(BUILD)keydb

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(BUILD)%.o: $(SRC)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)main.o: $(SRC)parse.h $(SRC)memorydb.h
$(BUILD)parse.o: $(SRC)parse.h $(SRC)keydbstring.h
$(BUILD)fileio.o: $(SRC)fileio.h
$(BUILD)memorydb.o: $(SRC)memorydb.h $(SRC)fileio.h $(SRC)hashtable.h $(SRC)keydbstring.h
$(BUILD)hashtable.o: $(SRC)hashtable.h $(SRC)keydbstring.h
$(BUILD)keydbstring.o: $(SRC)keydbstring.h

clean:
	rm -rf $(BUILD)
