# CPPy, a Python transpiler
CPPy is a work-in-progress Python to C++ transpiler.


## Example program
To test the Python implementation, you can try compiling this C++ program

```c++
#include "cpy.h"
#include <iostream>

int main()
{
    cpy::globals::Traceback::the().push("main");
    try {
        auto s1 = cpy::helpers::new_string("hello world");
        auto s2 = cpy::helpers::new_string("python");
        auto sep = cpy::helpers::new_string(", ");
        cpy::helpers::call(
            cpy::globals::BuiltinFunctions::the().get_function_named("print"),
            cpy::FunctionArguments({s1, s2, {sep, "sep"}})
        );
        cpy::globals::Traceback::the().pop();
    }
    catch (cpy::globals::traceback_exception& e) {
        std::cout << e.what();
    }
}
```