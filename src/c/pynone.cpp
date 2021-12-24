#include "pynone.h"
#include "pystring.h"
#include "pyfunction.h"
using namespace cpy;
#include <iostream>

PyNone::PyNone()
{
    std::vector<std::string> param_names;
    param_names.push_back("self");

    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__", param_names));
    setattr("__repr__", std::make_shared<PyFunction>(__repr__, "__repr__", param_names));
}

PyObjectPtr PyNone::__str__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}

PyObjectPtr PyNone::__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}