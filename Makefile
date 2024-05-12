# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -pthread

# Source files
SRC_FILES = defs.h house.c ghost.c hunter.c main.c logger.c utils.c

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
TARGET = ghost_hunting_simulation

# Default target
all: $(TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Clean target to remove object files and executables
clean:
	rm -f $(OBJ_FILES) $(TARGET)
