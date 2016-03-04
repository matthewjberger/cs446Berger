###########################################################
# Makefile Settings                                       #
###########################################################

# Suppress output except for errors and warnings
.SILENT:

# Executable name
EXE_NAME = sim01

# Compiler flags
COMPILER_FLAGS = -c -std=c++14 -g -Wall

# Linker flags
LINKER_FLAGS = -pthread

# Compiler being used
CC = g++

# Source file directory
SRC_DIR = .

# Find all source files
SRC= $(shell find $(SRC_DIR) -name '*.cpp')

# List object files
OBJECTS = $(SRC:%.cpp=%.o)

###########################################################
# Build System                                            #
###########################################################
all: $(EXE_NAME)

# Build executable from object files
$(EXE_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LINKER_FLAGS) -o $(EXE_NAME)

# Create object files from source files
%.o: %.cpp
	$(CC) $(COMPILER_FLAGS) $< -o $@

# Remove all object files
clean: $(OBJECTS)
	-rm $(OBJECTS)
