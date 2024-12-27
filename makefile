# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -I./src/core -I./src/modules -I./src/utils

# Directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
MODULES_DIR = $(SRC_DIR)/modules
UTILS_DIR = $(SRC_DIR)/utils

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(CORE_DIR)/person.cpp \
       $(MODULES_DIR)/user.cpp \
       $(MODULES_DIR)/driver.cpp \
       $(UTILS_DIR)/logger.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = build/SmartRide

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for compiling .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJS) $(TARGET) build

# Phony targets
.PHONY: all clean
