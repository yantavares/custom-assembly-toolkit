# Linguagem Inventada - liasm (EN)

<img src="utils/brazil.png" height="20px" width="auto" alt="Brazil Flag"  /> For the Portuguese version, click [here](README-pt.md).

Welcome to the repository of the Linguagem Inventada, a simulation machine that allows assembling and running programs written in the "Linguagem Inventada" or `liasm`, created in the University of Brasília (UnB).

## Compiler Workflow

The `liasm` compiler operates in two passes:

1. **First Pass**: The compiler reads the source `.liasm` file to build a table of labels, which is used to resolve addresses during the assembly process. During this phase, the compiler also performs lexical analysis to identify any syntax errors in the source code.

2. **Second Pass**: With the label table constructed, the compiler reads the source file a second time, this time translating the assembly instructions into binary code. The compiler also performs semantic checks to ensure the integrity of the program, such as validating instruction formats and detecting any undefined references.

## Project Structure

Here's an overview of the project structure to help you navigate and understand its components:

- **`liasm/`**: The main directory containing all the resources needed for the assembler and simulation.
  - **`bin/`**: Stores the compiled binaries of the assembler.
  - **`examples/`**: Contains examples of Inventive Assembly code for testing and learning.
  - **`includes/`**: Headers of libraries used by the assembler.
  - **`src/`**: Source code of the assembler.
  - **`tests/`**: Automated tests.
  - **`utils/`**: Auxiliary tools.
  - **`compiler.cpp`**: The main file of the compiler.
  - **`Makefile`**: To compile the project using `make`.
  - **`MEMdata.txt`** and **`MEMtext.txt`**: Simulate memory reserved for data and instructions, respectively.
- **`.gitignore`**: List of files/directories to ignore in versioning.
- **`README.md`**: Documentation (this file).

## Getting Started

### Prerequisites

Before you begin, ensure the following tools are installed on your system:

- **`make`**: Utility for automation of compilation.
- **`g++`**: The C++ compiler from the GNU Compiler Collection (GCC).

The assembler was tested succesafuly on OSX and Linux (both x86 and ARM).

### Assembling and Running Programs with `licc`

To compile the `licc` assembler:

1. **Navigate to the project directory**:

   ```bash
   cd liasm/
   ```

2. **Compile the assembler** using the `make` command:

   ```bash
    make
   ```

Or you can just use the precompiled binary in the `liasm` directory.

#### Using Makefile

After compiling the assembler:

- **To assemble a program**, replace `<file path>` with your `.asm` file:

  ```bash
  ./licc <file path>
  ```

- **Optional**: You can also set the mode to `debug` to see the generated labels and verbose output.

  ```bash
  ./licc <file path> debug
  ```

#### Direct Execution with `make run`

- **To compile the assembler and execute directly**, replace `<file path>` with the path of your `.asm` file:

  ```bash
  make run FILE=<file path>
  ```

- **Optional**: You can also set the mode to `debug` to see the generated labels and verbose output.

  ```bash
  make run FILE=<file path> MODE=debug
  ```

### Running Tests

To ensure the assembler is working correctly:

```bash
make test
```

This command compiles the assembler and runs all the tests in the `tests/` folder.

## License

This project is under the UNLICENSE, allowing complete freedom for use, modification, and distribution.
