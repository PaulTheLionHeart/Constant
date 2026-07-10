# Constant

**Constant** is a Win32 application for calculating mathematical constants to arbitrary precision.

Originally written as a numerical experiment and educational tool, the program demonstrates a collection of elegant algorithms for computing transcendental and irrational constants to many thousands (or even millions) of decimal places.

---

## Current Status

This repository contains the modernised x64 version of the original program.

Recent improvements include:

- x64 conversion
- Modern Win32 callback signatures
- Removal of obsolete Win16 declarations
- Warning-free Visual Studio build
- Project reorganised into `src/`
- Copy-to-Clipboard support
- Generic square-root engine
- Catalan's Constant
- Apéry's Constant ζ(3)
- 64-bit arbitrary-precision multiplication and division helpers
- Improved output formatting and keyboard navigation

---

## Output Formats

The on-screen display and the **Copy** button provide the raw decimal expansion, making it easy to paste the digits directly into other applications.

The **Save As** option produces a formatted text file with grouped digits and line numbers for improved readability.

Future versions may provide both raw and formatted output options for either destination.

---

## Implemented Constants

- π (Pi)
- e (Natural logarithm base)
- e (Fast alternative algorithm)
- φ (Golden Ratio)
- √n (Generic square-root engine supporting √2–√20)
- Natural logarithms ln(n)
- Catalan's Constant
- Apéry's Constant ζ(3)

---

## References

The `references` directory contains papers and reference implementations consulted during the development of the newer algorithms.

The implementations in this program are **independent translations** into Pascal Sebah's arbitrary-precision arithmetic framework. The reference material is included to document the underlying mathematics and to preserve the historical background behind the implemented algorithms.

---

## Building

The project currently builds with Visual Studio 2017 and later.

CMake support is planned for a future release.

---

## Philosophy

The aim of this project is to preserve the original algorithms while carefully modernising the codebase and user interface, keeping the program simple, educational and easy to understand.

Rather than becoming an encyclopaedia of mathematical constants, the program presents a curated collection of significant constants, each implemented using elegant arbitrary-precision algorithms that illustrate different areas of mathematics.

---

## Credits

### Paul the LionHeart

Author, developer, workshop engineer, and relentless dragon tamer.

### ChatGPT

Workshop assistant, reviewer, teacher, sounding board, mathematical collaborator, and enthusiastic dragon spotter.

---

> *"Every bug has a cause. Every algorithm has a story. Every dragon can be tamed."*