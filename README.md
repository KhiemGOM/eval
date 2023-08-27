# Math Expression Evaluator


A simple and extensible math expression evaluator written in C++ that allows you to parse and evaluate mathematical expressions. This project is inspired by the functionality of popular calculator apps and libraries.

## Features

- Parses mathematical expressions and evaluates them according to standard arithmetic rules.
- Supports basic operators like addition, subtraction, multiplication, and division.
- Handles parentheses and respects operator precedence.
- Support many functions and constant (please suggest new one if you see some missing)

## Usage
You can download this and `#include "eval.h"` for your personal project or download the release and open the .exe file to play with this.
Example expression:
```c++
sin(pi / 6) + cos(pi / 3)
```
```c++
(((sin(pi/4) + cos(pi/3)) * (log(10, 100) + sqrt(25))) / ((exp(0.5) + tan(pi/6)) * ((log(3, 27) - sinh(0.3)) + (cosh(0.7) * atan(1)))))
```
