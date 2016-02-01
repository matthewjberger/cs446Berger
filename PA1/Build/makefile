###########################################################
# Makefile Settings                                       #
###########################################################

# Compiler being used
CC = gcc

# Output directory
OUT_DIR= ../Bin/

# Convert all source files to object files
OBJECTS = $(SRC:%.cpp=%.o)

# Source directory
SRC_DIR=../Source

# Find all source files
SRC= $(shell find $(SRC_DIR) -name *.cpp)

# Executable name
EXE_NAME = Project1

# INCLUDE_PATHS specifies additional include paths
INCLUDE_PATHS=

# LIBRARY_PATHS specifies additional library paths
LIBRARY_PATHS=

# Linker flags
LINKER_FLAGS=

# Compiler flags
COMPILER_FLAGS= -Wall -std=c++11

###########################################################
# Build System                                            #
###########################################################
all: $(EXE_NAME)

$(EXE_NAME): $(OBJECTS)
	mkdir -p $(OUT_DIR) \
	&& $(CC) $(OBJECTS) $(LINKER_FLAGS) -o $(OUT_DIR)$(EXE_NAME)

%.o: %.cpp
	$(CC) -c $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $< -o $@

clean: $(OBJECTS)
	-rm $(OBJECTS)
