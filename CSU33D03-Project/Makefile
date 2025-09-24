CXX = g++
CXXFLAGS = -Wall -std=c++17

BUILD_DIR = test_builds
SHARED = ../shared

# General rule
all: 
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/main $(wildcard *.cpp) $(wildcard $(SHARED)/*.cpp)

# Ensure the directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rule to remove object files and the executable
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: all clean