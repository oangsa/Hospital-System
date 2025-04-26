OS := $(shell uname -s)

CXX := g++
CXXFLAGS := -Wall -Wextra

ifeq ($(OS),Linux)
	EXE := main
else ifeq ($(OS),Windows_NT)
	EXE := main.exe
	CXXFLAGS += -DWIN32
endif

SRC_DIRS := . Features Libs
OBJ_DIR := obj

SRCS := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJ_DIR) $(EXE)

run: all
	@./$(EXE)
	@$(MAKE) clean

.PHONY: all clean
