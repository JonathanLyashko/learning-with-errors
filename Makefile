# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17

# Libraries
LIBS = -lntl -lgmp

# Target executable
TARGET = main

# Build target
all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LIBS)

# Clean target
clean:
	rm -f $(TARGET)