# CS271 Project 6 - Hack Assembler

## Author
Brent Grossman

## Description
This is a two-pass assembler for the Hack assembly language that translates .asm files into .hack binary files.

## Files
- `main.c` - **Driver program** (contains main function)
- `parser.c` / `parser.h` - Parsing and assembly functions
- `symtable.c` / `symtable.h` - Symbol table implementation
- `hack.h` - Hack language definitions and lookup tables
- `error.c` / `error.h` - Error handling
- `Makefile` - Build configuration

## Driver Program
The driver program is **main.c**. This file contains the main() function that:
1. Takes a .asm filename as a command-line argument
2. Calls the parse() function to perform the first pass
3. Calls the assemble() function to perform the second pass and generate the .hack output file

## How to Compile
Run the following command in the 06 directory:
```
make
```

This will compile all source files and create an executable named `main`.

## How to Use the Assembler
The assembler expects exactly one command-line argument: the path to a .asm file.

**Basic usage:**
```
.\main <path-to-asm-file>
```

**Examples:**
```
.\main asm\Add.asm
.\main asm\Max.asm
.\main asm\Pong.asm
```

**What happens:**
- Input: `asm\Add.asm`
- Output: `asm\Add.asm.hack` (created in the same directory as the input file)

The assembler will:
- Read the .asm file
- Perform two-pass assembly (symbol table construction, then code generation)
- Write binary machine code to a .hack file with the same name as the input file plus ".hack" extension
- Display error messages if the input file cannot be opened or contains invalid instructions

## Testing
Test files are provided in the `asm\` subdirectory:
- `Add.asm` - Simple addition program
- `Max.asm` / `MaxL.asm` - Find maximum of two numbers (with/without symbols)
- `Rect.asm` / `RectL.asm` - Draw a rectangle (with/without symbols)
- `Pong.asm` / `PongL.asm` - Complete Pong game (with/without symbols)

Run each test file and verify the output .hack files are created.
