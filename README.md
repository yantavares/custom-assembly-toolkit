# Liasm Assembly Toolkit

:brazil: **For Portuguese version, read [README-pt.md](README-pt.md).**

**Note:** If this repository helped you, please leave a :star: on the repo :)

## Overview

The Custom Assembly Toolkit is a comprehensive solution for working with the liasm assembly language and transpilating it to IA-32 assembly. The `Liasm` assembly language is a hypothetical assembly language created at UnB. The language itself has no name, and so I took the liberty to call it `Liasm`. The toolkit includes two main components:

1. **Assembler (+ linker)**: Converts liasm source code into machine code for the hypothetical architecture. It also links 2 object files into a single executable.
2. **Transpiler**: Translates liasm assembly code into IA-32 assembly language.

This repository serves as a central hub, providing an entry point to both tools, along with detailed documentation and usage instructions.

## liasm Assembly Language

The liasm environment emulates a simple computer architecture with the following core components:

- **ACC**: A 16-bit accumulator register for arithmetic and logic operations.
- **PC**: A 16-bit program counter register that points to the next instruction.
- **Memory**: A memory array with \( 2^{16} \) words, each 16 bits wide.

### Instruction Formats

The liasm language supports three instruction formats:

1. **Format 1**: Single Opcode
2. **Format 2**: Opcode followed by one address
3. **Format 3**: Opcode followed by two addresses

This minimal yet complete instruction set allows the implementation of various algorithms, making liasm a Turing-complete language.

### Instruction Set

| Mnemonic   | Opcode | Size (words) | Action                                                  |
|------------|--------|--------------|---------------------------------------------------------|
| ADD        | 01     | 2            | `ACC <- ACC + mem(OP)`                                  |
| SUB        | 02     | 2            | `ACC <- ACC - mem(OP)`                                  |
| MUL        | 03     | 2            | `ACC <- ACC * mem(OP)`                                  |
| DIV        | 04     | 2            | `ACC <- ACC ÷ mem(OP)`                                  |
| JMP        | 05     | 2            | `PC <- OP`                                              |
| JMPN       | 06     | 2            | `If ACC<0 then PC <- OP`                                |
| JMPP       | 07     | 2            | `If ACC>0 then PC <- OP`                                |
| JMPZ       | 08     | 2            | `If ACC=0 then PC <- OP`                                |
| COPY       | 09     | 3            | `mem(OP2) <- mem(OP1)`                                  |
| LOAD       | 10     | 2            | `ACC <- mem(OP)`                                        |
| STORE      | 11     | 2            | `mem(OP) <- ACC`                                        |
| INPUT      | 12     | 2            | `mem(OP) <- input`                                      |
| OUTPUT     | 13     | 2            | `output <- mem(OP)`                                     |
| STOP       | 14     | 1            | Suspends execution                                      |
| S_INPUT*   | 15     | 1            | Receives a label to input and number of bytes to read   |
| S_OUTPUT*  | 16     | 1            | Receives a label to output and number of bytes to write |

\* *S_INPUT and S_OUTPUT are system calls that interact with the transpiler and do not work in the assembler.*

### Directives

- **CONST**: Allocates a constant value to a memory address.  
  Example: `N3: CONST 1 ; Allocates 1 to N3`

- **SPACE**: Allocates memory space to an address without initializing it.  
  Example: `N1: SPACE ; Allocates memory to N1 address`

### Comments

Comments in liasm start with a semicolon (`;`). Everything following the `;` on a line is considered a comment and is ignored by the assembler.

## Toolkit Components

### 1. Assembler
The assembler translates liasm source code into the machine code of the hypothetical architecture. It operates in a single-pass compilation process, ensuring a straightforward and efficient conversion.

**[Assembler README](assembler/README.md)**: Detailed information on the assembler can be found in the `assembler` folder.

### 2. Transpiler
The transpiler converts liasm assembly code into IA-32 assembly language, allowing the hypothetical programs to be executed on x86 architectures. Input and output operations in the transpiled code are implemented using IA-32 assembly instructions, ensuring compatibility with standard x86 I/O mechanisms.

**[Transpiler README](transpiler/README.md)**: Detailed information on the transpiler can be found in the `transpiler` folder.

## Compilation Workflow

1. **Single-Pass Compilation**: The assembler processes liasm source code in a single pass, generating the hypothetical machine code.
2. **Transpilation**: The transpiler converts the liasm assembly into IA-32 assembly, preserving the logic and structure of the original code, and implementing input/output operations using IA-32 assembly.

## Getting Started

To get started, please refer to the README files in the respective folders:

- [Assembler README](assembler/README.md)
- [Transpiler README](transpiler/README.md)

These documents provide detailed instructions on building, configuring, and using the tools.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.


## Acknowledgments

Special thanks to (Guts_Santana)[https://github.com/Guts-Santana] for making this project possible.

