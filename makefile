CXX = g++
CXXFLAGS = -std=c++17 -g -I/mingw64/include
LDFLAGS = -L/mingw64/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static

SRC_DIR = src
MODULES_DIR = $(SRC_DIR)/modules
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(SRC_DIR)/main.cpp \
	$(MODULES_DIR)/person.cpp \
	$(MODULES_DIR)/user.cpp \
	$(MODULES_DIR)/driver.cpp \
	$(MODULES_DIR)/edge.cpp \
	$(MODULES_DIR)/node.cpp \
	$(MODULES_DIR)/vehicle.cpp
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/SmartRide.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean