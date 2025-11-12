# Clike-Complier

## Overview
This project implements a simple **one-pass compiler** for a C-like programming language.  
It covers the fundamental stages of compiler design — from lexical and syntax analysis to intermediate and target code generation.

The compiler reads user-defined grammar rules and a source program, then analyzes and translates the source code into **MIPS assembly**.


## Input
The compiler takes two main input files:

- **`grammar.txt`** – Defines the grammar of the C-like language using production rules.  
- **`code_in.txt`** – Contains the source program written according to the grammar rules.


## Output
The compilation process produces multiple files that represent different stages:

### Lexical & Syntax Analysis
- `Pre-Processed_Code.txt` – Source code after preprocessing (comments and extra spaces removed).  
- `Lexical_Result.txt` – Tokenized results.  
- `Tables.csv` – ACTION and GOTO parsing tables.  
- `Parse_Tree.png` – Visualization of the syntax tree.

### Intermediate & Target Code
- `Quaternion_List.txt` – Intermediate representation using quadruples.  
- `Target_Code.asm` – Final MIPS assembly code, executable in **Mars**.


## Features
The compiler performs a complete end-to-end compilation process, including:

1. **Grammar Analysis** – Reads and processes grammar definitions to construct parsing tables.  
2. **Lexical Analysis** – Scans the source code and generates tokens.  
3. **Syntax Analysis** – Performs shift-reduce parsing using ACTION and GOTO tables.  
4. **Semantic Analysis** – Detects semantic errors such as:
   - Undefined or redeclared variables  
   - Missing `main` function  
   - Invalid function return or parameter types  
5. **Intermediate Code Generation** – Produces quadruple-based intermediate representation with basic block division and liveness analysis.  
6. **Target Code Generation** – Converts intermediate code into **MIPS assembly**, managing registers and stack frames.


## Notes
The project aims to provide a clear and modular implementation of a compiler pipeline, helping students understand the design and logic behind each stage of compilation.


## Example
```bash
# Example usage
python compiler.py grammar.txt code_in.txt
# Output: Target_Code.asm (runnable in Mars)
