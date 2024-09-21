# Define the compiler, assembler, and linker
CXX = g++
NASM = nasm
LD = ld

# Directories for binaries and output files
BIN_DIR = bin
OUTPUT_DIR = output

# Ensure the directories exist
.PHONY: dirs all clean

# Default target
all: dirs $(BIN_DIR)/tradutor run_tradutor assemble link run_binary

# Create bin and output directories if they don't exist
dirs:
	mkdir -p $(BIN_DIR) $(OUTPUT_DIR)

# Compile Tradutor.cpp into bin/
$(BIN_DIR)/tradutor: Tradutor.cpp
	$(CXX) Tradutor.cpp -o $(BIN_DIR)/tradutor

# Run the tradutor binary
run_tradutor:
	./$(BIN_DIR)/tradutor arquivos/$(FILE)

# Assemble the .asm file, placing object files in output/
assemble:
	$(NASM) -f elf32 $(OUTPUT_DIR)/$(FILE).s -o $(OUTPUT_DIR)/$(FILE).o

# Link the object file to create the final binary in bin/
link:
	$(LD) -m elf_i386 -o $(BIN_DIR)/$(FILE) $(OUTPUT_DIR)/$(FILE).o

# Run the final binary
run_binary:
	./$(BIN_DIR)/$(FILE)

# Clean up generated files
clean:
	rm -f $(BIN_DIR)/* $(OUTPUT_DIR)/*
