CC = gcc
CFLAGS = -MMD -Wall

# Directories
SRCDIR = src
BUILDDIR = build

TARGET = $(BUILDDIR)/keydb

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
