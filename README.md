# Runic: L-System & Turtle Graphics Engine

## Overview
Runic is an interactive C-based engine designed to parse and visualize Lindenmayer systems (L-systems). It features a custom Turtle graphics implementation for rendering complex fractal structures (referred to as "runes") onto raster images. Additionally, the engine supports custom typography by parsing Glyph Bitmap Distribution Format (BDF) fonts and rendering text directly onto the generated graphics.

## Technical Capabilities
This project demonstrates advanced proficiency in C programming, specifically highlighting:
- **Custom Data Structures**: Implemented a dynamic array-based stack for the Turtle's spatial state and a linked-list to manage the application's state history, enabling robust and infinite `UNDO` and `REDO` functionality.
- **Manual Memory Management**: Rigorous dynamic memory allocation (`malloc`, `calloc`, `realloc`) is used throughout. This includes complex deep-copy algorithms for preserving application state transitions (cloning 2D image matrices, recursive L-system rules, and font maps). Careful memory deallocation is implemented and rigorously verified for leaks using **Valgrind** to ensure a robust, leak-free environment.
- **Pointer Arithmetic & String Parsing**: Extensive use of pointer arithmetic and custom string manipulation functions to tokenize commands, recursively derive L-system rules, and safely process text and binary file streams.
- **Code Modularity**: The system architecture is heavily decoupled. Domain-specific logic (image processing, computational geometry, typography, and state management) is separated into distinct, well-defined modules with clear header interfaces, resulting in maintainable and extensible code.

## Core Features
- **L-System Derivation**: Load axioms and production rules from `.lsys` files and derive complex instruction strings up to $n$ iterations.
- **Turtle Graphics Engine**: 
  - Interprets instruction strings to render geometric shapes.
  - Implements Bresenham's Line Algorithm from scratch to rasterize continuous mathematical lines into discrete pixels.
  - Supports state saving and restoration using stack operations (interpreting `[` and `]` symbols for branching fractal structures).
- **Interactive State Management**: 
  - Complete `UNDO` and `REDO` mechanisms capable of reverting non-trivial mutations to the program state (such as drawing, deriving, and loading files).
- **Typography & BDF Parsing**: Loads and parses bitmap fonts from `.bdf` files, executing bitwise decoding of hex-encoded glyphs into dynamic 2D matrices, and projecting text onto the active image buffer.
- **PPM Image Processing**: Handles direct binary and ASCII I/O for the Netpbm (PPM P6) format, facilitating image loading, saving, and pixel-level matrix manipulation.

## Architecture
The codebase is structured into cohesive modules:
- `main.c`: The entry point and interactive command loop. Handles user input parsing, routing, and coordinates state history management.
- `lsystems.c`: Responsible for reading `.lsys` rule files and generating derived strings based on the fractal production rules.
- `turtle.c`: Contains the computational geometry for the Turtle graphics, including trigonometric coordinate calculation, the drawing stack, and Bresenham's rasterization algorithm.
- `fonts.c`: Handles parsing of BDF typography files and the logic required to overlay characters dynamically onto the image canvas.
- `image.c`: Implements the serialization and deserialization of the binary PPM image format, mapping bytes to the internal pixel matrix.
- `utils.c`: A robust utility suite containing custom string operations, deep-copy routines for the `programstate` structure, and dynamic memory cleanup mechanisms.
- `structs.h`: Centralizes all data models and type definitions, keeping data representations uniform across the system.

## Getting Started

### Compilation
The project relies on a standard `Makefile` for compilation. To build the executable, run:
```bash
make build
```
This will compile the source code and generate an executable named `runic`.

### Execution
Run the program without any arguments to enter the interactive shell:
```bash
./runic
```

### Example Usage
Once inside the interactive prompt, you can issue commands to generate fractals:
```text
LOAD blank_800x800.ppm
LSYSTEM plant.lsys
TURTLE 400 50 8 90 25 5 0 55 0
SAVE my_plant.ppm
EXIT
```
*(The above script loads a blank canvas, reads the `plant.lsys` rules, computes 5 iterations of the L-system, renders a fractal tree in dark green using the turtle starting at coordinates (400, 50), and saves the resulting buffer to a new file.)*

