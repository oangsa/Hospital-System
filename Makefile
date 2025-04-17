# Detect OS
OS := $(shell uname -s)

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra

# Executable based on OS
ifeq ($(OS),Linux)
	EXE := main
else ifeq ($(OS),Windows_NT)
	EXE := main.exe
	CXXFLAGS += -DWIN32
endif

# Directories to search for source files
SRC_DIRS := . Features Libs
OBJ_DIR := obj

# Find all .cpp files in the listed directories
SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Default rule
all: $(EXE)

# Link all object files into the final executable
$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp into .o, keeping directory structure in obj/
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Auto-include dependency files
-include $(DEPS)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(EXE)

run: all
	@./$(EXE)
	@$(MAKE) clean

.PHONY: all clean

