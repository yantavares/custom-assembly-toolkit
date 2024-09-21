# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

# Source and header files
SRCS = main.cpp $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.h)
OBJS = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Directories
BIN_DIR = .
OBJ_DIR = obj

# Target executable
TARGET = $(BIN_DIR)/montador

# Default target
all: $(TARGET)

# Rule for creating the target executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for creating object files
$(OBJ_DIR)/%.o: src/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create the bin and obj directories if they do not exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Accept filename and preprocess/compile .asm files
as: $(TARGET)
	./$(TARGET) -p $(file)
	./$(TARGET) -o $(basename $(file)).pre

# Link two files after preprocessing
ld:  $(TARGET)
	./$(TARGET) -p $(file1)
	./$(TARGET) -o $(basename $(file1)).pre
	./$(TARGET) -p $(file2)
	./$(TARGET) -o $(basename $(file2)).pre
	./$(TARGET) -l $(basename $(file1)).e $(basename $(file2)).e

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rm -rf $(BIN_DIR)/$(TARGET) $(OBJ_DIR)
	rm -f Arquivos/*.obj Arquivos/*.pre Arquivos/*.e
	rm -f *.obj *.pre *.e

# Phony targets
.PHONY: all clean as link
