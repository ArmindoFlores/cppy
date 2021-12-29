# CPPy, a Python transpiler
CPPy is a work-in-progress Python to C++ transpiler.

----

## Project Structure
CPPy is made up of a C++ library - libcppy - and a Python package. The library is used to implement all basic python functionality, as well as most of the built-ins. The Python package will be responsible for parsing a file and outputting the actuall C++ code, as well as running all the necessary commands for compilation.

## Current Status
Currently, there is almost no work done on the Python package side. This is because, without having a (semi) working Python implementation, it is very hard to guess how we might want to translate a piece of code. Of course, there is a lot that can be done in parallel: for example, the parser and lexer are completely independent from the Python implementation.

On the C++ side, there exists the beginning of Python objects (`PyObject` class) and some of the built-in types (`type`, `int`, `str`, `list`, `function`). However, these types don't yet have all of their functionality implemented. There are also some built-in functions, like `print` and `id`. Exceptions exist but are not Python objects for the moment. There is a *very* rudimentary garbage collector for dealing with cyclic references, and everything else is handled (memory-wise) using `std::shared_ptr`. There are no optimizations implemented yet, and there are no plans of implenting any until there is a working unoptimized prototype.

## Example program
There is no standard way to build this project (yet), but to test the Python implementation you can try compiling this C++ program (and all the source code in `src/c`)

```c++
#include "cppy.h"
#include <iostream>

int main()
{
    cppy::globals::Traceback::the().push("main");
    try {
        // s1 = "hello world"
        auto s1 = cppy::helpers::new_string("hello world");
        // s2 = "python"
        auto s2 = cppy::helpers::new_string("python");
        // sep = ", "
        auto sep = cppy::helpers::new_string(", ");
        // print(s1, s2, sep=sep)
        cppy::helpers::call(
            cppy::globals::BuiltinFunctions::the().get_function_named("print"),
            cppy::FunctionArguments({s1, s2, {sep, "sep"}})
        );
        cppy::globals::Traceback::the().pop();
    }
    catch (cppy::globals::traceback_exception& e) {
        std::cout << e.what();
    }
}
```

## Python Grammar
The Python grammar file (`src/transpiler/Python3.g4`) was written by Bart Kiers. [Here](ttps://github.com/bkiers/python3-parser) is a link to their project.

## How to help
You can help by submitting a pull requested with add fixes or new functionality, or by opening issues regarding bugs that you find. **Please do not open issues about missing features/feature requests.**