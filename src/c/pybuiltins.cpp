#include <iostream>
#include "pybuiltins.h"
#include "pyfunction.h"
#include "pystring.h"
#include "pytraceback.h"
#include "pyhelpers.h"
using namespace cpy;

PyObjectPtr builtins::__print__(PyObjectPtr obj)
{
    cpy::globals::Traceback::the().push("print");
    PyObjectPtr result = helpers::call_member("__str__", obj, FunctionArguments());
    PyString *s = dynamic_cast<PyString*>(result.get());

    if (s == nullptr) cpy::globals::Traceback::the().raise("TypeError: __str__() must return a string", "TypeError");

    std::cout << s->internal << "\n";
    cpy::globals::Traceback::the().pop();
    return obj;
}