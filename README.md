# CPPy, a Python transpiler
CPPy is a work-in-progress Python to C++ transpiler.


## Example program
To test the Python implementation, you can try compiling this C++ program

```c++
#include "cppy.h"
#include <iostream>

int main()
{
    cppy::globals::Traceback::the().push("main");
    try {
        auto s1 = cppy::helpers::new_string("hello world");
        auto s2 = cppy::helpers::new_string("python");
        auto sep = cppy::helpers::new_string(", ");
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