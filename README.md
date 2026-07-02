# Constant

Constant is a Win32 application for calculating mathematical constants to arbitrary precision.

Originally written as a numerical experiment and educational tool, the program demonstrates several different algorithms for computing transcendental and irrational constants to many thousands (or millions) of decimal places.

## Current Status

This repository contains the modernised x64 version of the original program.

Recent improvements include:

- x64 conversion
- Modern Win32 callback signatures
- Removal of obsolete Win16 declarations
- Warning-free Visual Studio build
- Project reorganised into `src/`
- Copy-to-Clipboard support

### Output formats

The on-screen display and the **Copy** button provide the raw decimal expansion, making it easy to paste the digits directly into other applications.

The **Save As** option produces a formatted text file with grouped digits and line numbers for improved readability.

Future versions may provide both raw and formatted output options for either destination.

## Implemented Constants

- π (Pi)
- e (Natural logarithm base)
- e (Fast alternative algorithm)
- φ (Golden Ratio)
- √2
- Natural logarithms

## Planned Improvements

- Additional mathematical constants
- Alternative output formats
- Raw/Formatted export options
- CMake build support

## Building

The project currently builds with Visual Studio 2017 and later.

## Philosophy

The aim of this project is to preserve the original algorithms while carefully modernising the codebase and user interface, keeping the program simple, educational and easy to understand.