# Compiler and flags
CC = gcc
#CFLAGS = -Wall -Wextra


# Find all source files
SRCS = $(wildcard *.c)

# Target executable name
TARGET = ifjcompiler

# Default target
all: $(TARGET)

$(TARGET): 
	#$(CC) $(CFLAGS) $(SRCS) -o $@
	$(CC) $(SRCS) -o $@

# Clean up
clean:
	rm -f $(TARGET)

.PHONY: all clean
