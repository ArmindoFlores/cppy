# CPPy, a Python transpiler
CPPy is a work-in-progress Python to C++ transpiler.

----

## Project Structure
CPPy is made up of a C++ library - libcppy - and a Python package. The library is used to implement all basic python functionality, as well as most of the built-ins. The Python package will be responsible for parsing a file and outputting the actuall C++ code, as well as running all the necessary commands for compilation.

## Current Status
Currently, there is almost no work done on the Python package side. This is because, without having a (semi) working Python implementation, it is very hard to guess how we might want to translate a piece of code. Of course, there is a lot that can be done in parallel: for example, the parser and lexer are completely independent from the Python implementation.

On the C++ side, there exists the beginning of Python objects (`PyObject` class) and some of the built-in types (`type`, `int`, `str`, `list`, `function`). However, these types don't yet have all of their functionality implemented. There are also some built-in functions, like `print`, `input`, and `id`. Exceptions exist but are not Python objects for the moment. There is a *very* rudimentary garbage collector for dealing with cyclic references, and everything else is handled (memory-wise) using `std::shared_ptr`. There are no optimizations implemented yet, and there are no plans of implenting any until there is a working unoptimized prototype.

## Build Instructions
To build the project, ensure you have Java and the [ANTLR4 tool](https://www.antlr.org/download.html). Then, run the following commands:
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `sudo make install`
This will compile all the required libraries and install the Python package. You can then use cppy from the command line using `python3 -m cppy`.

## Python Grammar
The Python grammar file (`src/transpiler/Python3.g4`) was written by Bart Kiers. [Here](https://github.com/bkiers/python3-parser) is a link to their project.

## How to help
You can help by submitting a pull requested with add fixes or new functionality, or by opening issues regarding bugs that you find. **Please do not open issues about missing features/feature requests.**