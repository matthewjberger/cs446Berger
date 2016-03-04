###########################################################
# Makefile Settings                                       #
###########################################################

# Suppress output except for errors and warnings
.SILENT:

# Executable name
EXE_NAME = sim01

# Compiler flags
COMPILER_FLAGS = -c -std=c++14 -g -Wall -pthread

# Compiler being used
CC = g++

# Files to compile
OBJECTS = *.cpp

###########################################################
# Build System                                            #
###########################################################
all: $(EXE_NAME)

$(EXE_NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXE_NAME)

%.o: %.cpp
	$(CC) $(COMPILER_FLAGS) $< -o $@

clean: $(OBJECTS)
	-rm $(OBJECTS)
