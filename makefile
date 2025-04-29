CXX = g++
CXXFLAGS = -std=c++14 -Wall -pthread -IHeader_Files
SRC_DIR = Source_Files
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/event_processor

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Main target
all: directories $(TARGET)

# Create necessary directories
directories:
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Compile the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Dependencies
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp Header_Files/Event.h Header_Files/EventQueue.h Header_Files/ThreadPool.h Header_Files/Log.h Header_Files/EventProcessors.h
$(OBJ_DIR)/EventQueue.o: $(SRC_DIR)/EventQueue.cpp Header_Files/EventQueue.h Header_Files/Event.h
$(OBJ_DIR)/ThreadPool.o: $(SRC_DIR)/ThreadPool.cpp Header_Files/ThreadPool.h
$(OBJ_DIR)/EventProcessors.o: $(SRC_DIR)/EventProcessors.cpp Header_Files/EventProcessors.h Header_Files/Event.h Header_Files/Timer.h Header_Files/Log.h
$(OBJ_DIR)/Log.o: $(SRC_DIR)/Log.cpp Header_Files/Log.h
$(OBJ_DIR)/Timer.o: $(SRC_DIR)/Timer.cpp Header_Files/Timer.h Header_Files/Log.h

.PHONY: all clean directories