#include "pynone.h"
#include "pystring.h"
#include "pyfunction.h"
using namespace cpy;
#include <iostream>

PyNone::PyNone()
{
    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__"));
    setattr("__repr__", std::make_shared<PyFunction>(__repr__, "__repr__"));
}

PyObjectPtr PyNone::__str__(const FunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}

PyObjectPtr PyNone::__repr__(const FunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}