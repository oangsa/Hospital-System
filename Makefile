SHELL := /bin/bash

# Detect if we're on Windows or Linux
ifeq ($(OS),Windows_NT)
    USING_WINDOWS := 1
else
    USING_WINDOWS := 0
endif

ifeq ($(USING_WINDOWS),1)
    SHELL := cmd.exe
    .SHELLFLAGS := /C
    EXE := main.exe
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM = del /q
    RMDIR = rmdir /s /q
    RUNEXE = $(EXE)
else
    EXE := main
    MKDIR = mkdir -p $(1)
    RM = rm -f
    RMDIR = rm -rf
    RUNEXE = ./$(EXE)
endif

# Compiler settings
CXX := g++
CXXFLAGS := -Wall -Wextra

SRC_DIRS := . Features Libs
OBJ_DIR := obj

SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
SRCS := $(subst ./,,$(SRCS))

OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

vpath %.cpp $(SRC_DIRS)

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	@$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

clean:
	-$(RMDIR) $(OBJ_DIR)
	-$(RM) $(EXE)

run: all
	$(RUNEXE)
	$(MAKE) clean

.PHONY: all clean run
