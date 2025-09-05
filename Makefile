CC = gcc
CFLAGS = -MMD -Wall -Wextra -pedantic

# Directories
## installation
BINDIR=~/bin
DATADIR=~/pairdb-data

## source and object files
SRCDIR = src
BUILDDIR = build

EXE=pairdb

TARGET = $(BUILDDIR)/$(EXE)

.PHONY: all
all: $(TARGET)

# Files
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
DEPS = $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -o $@ $^

#%.o: %.c
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/

.PHONY: install
install:
	@mkdir -p $(BINDIR)
	@install $(TARGET) $(BINDIR)/$(EXE)
	@mkdir -p $(DATADIR)
