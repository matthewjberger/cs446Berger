###########################################################
# Makefile Settings                                       #
###########################################################

# Suppress output except for errors and warnings
.SILENT:

# Executable name
EXE_NAME = family

# Compiler flags
COMPILER_FLAGS = -Wall -g

# Compiler being used
CC = gcc

# Output directory
OUT_DIR = ../Bin/

# Convert all source files to object files
OBJECTS = $(SRC:%.c=%.o)

# Source directory
SRC_DIR =../Source

# Find all source files
SRC = $(shell find $(SRC_DIR) -name *.c)

# Additional include paths
INCLUDE_PATHS =

# Additional library paths
LIBRARY_PATHS =

# Linker flags
LINKER_FLAGS =

# Files/Directories to copy to output directory
OUT_FILES = $(SRC_DIR)/Data

###########################################################
# Build System                                            #
###########################################################
all: $(EXE_NAME)

$(EXE_NAME): $(OBJECTS) copyfiles
	mkdir -p $(OUT_DIR) \
	&& $(CC) $(OBJECTS) $(LINKER_FLAGS) -o $(OUT_DIR)$(EXE_NAME)

%.o: %.c
	$(CC) -c $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $< -o $@

clean: $(OBJECTS)
	-rm $(OBJECTS)

copyfiles:
	cp -r $(OUT_FILES) $(OUT_DIR)
