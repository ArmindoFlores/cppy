#include "pystring.h"
#include "pyfunction.h"
#include "helpers.h"
#include "pytraceback.h"
using namespace cpy;
#include <iostream>
#include <cctype>

PyString::PyString(const std::string &s) : internal(s) 
{
    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__"));
    setattr("capitalize", std::make_shared<PyFunction>(capitalize, "capitalize"));
}

PyObjectPtr PyString::__str__(const FunctionArguments &args)
{
    if (args.size() != 1) {
        globals::Traceback::the().raise("TypeError: expected 1 argument, got " + std::to_string(args.size()), "TypeError");
    }
    return args[0].get_object();
}

PyObjectPtr PyString::capitalize(const FunctionArguments &args)
{
    if (args.size() != 1) {
        globals::Traceback::the().raise("TypeError: expected 1 argument, got " + std::to_string(args.size()), "TypeError");
    }
    std::string new_str = args[0].get_object()->as<PyString>().internal;
    for (char &x : new_str)
        x = toupper(x);
    return helpers::new_string(new_str);
}