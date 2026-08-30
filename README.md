# s21_decimal

A custom C implementation of a 128-bit fixed-point decimal data type, designed to safely process financial data and high-precision calculations without the rounding errors typical of floating-point types.

## About The Project

In standard C, financial calculations using `float` or `double` often suffer from precision loss due to IEEE 754 binary representation. This library introduces a robust `s21_decimal` type (similar to C#'s `Decimal`), utilizing a 128-bit structure: 96 bits for the integer mantissa and 32 bits for the sign and scale factor.

### Architecture Highlight: `big_decimal`
To guarantee zero precision loss during intermediate arithmetic calculations (especially multiplication and division), the core engine utilizes a custom `big_decimal` structure. This expanded type safely handles arithmetic overflows before applying banker's rounding and scaling the result back into the standard 128-bit `s21_decimal` format.

## Features

* **Arithmetic Operations:** Addition, subtraction, multiplication, and division with overflow protection and banker's rounding.
* **Comparison Operators:** Full suite of comparison logic (`<`, `<=`, `>`, `>=`, `==`, `!=`).
* **Converters:** Seamless conversion between `s21_decimal` and standard C types (`int`, `float`).
* **Math Utilities:** Functions for rounding (`floor`, `round`), truncation, and negation.

## Technical Stack
* **Language:** C (C11 standard)
* **Build System:** Make
* **Testing:** `Check` framework with >90% coverage
* **Memory Safety:** Validated using Valgrind (zero leaks)

## Getting Started

### Prerequisites
Make sure you have GCC, Make, and the Check testing framework installed on your system.

### Build Instructions

The project includes a `Makefile` with the following targets:

* Build the static library:
  ```bash
  make s21_decimal.a

* Run the unit test suite:
  ```bash
  make test

* Generate an HTML coverage report (requires `gcov` and `lcov`):
  ```bash
  make gcov_report

## Project Structure

* `core/decimal/` — Implementations of standard 128-bit operations.
* `core/big_decimal/` — Intermediate extended-precision arithmetic engine to prevent overflow.
* `tests/` — Comprehensive unit test suite.

