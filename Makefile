CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -I/usr/include/SFML -Og
LDFLAGS = 
SRC_DIR = src
BUILD_DIR = build
TARGET = scapegoat.out

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard lib/scapegoat/*.cpp) \
          $(wildcard lib/*.cpp)

OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)

bear: clean
	bear -- make

.PHONY: all clean run bear
