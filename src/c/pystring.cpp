#include "pystring.h"
#include "pyfunction.h"
#include "pyhelpers.h"
#include "pytraceback.h"
using namespace cpy;
#include <iostream>
#include <cctype>

PyString::PyString(const std::string &s) : internal(s) 
{
    std::vector<std::string> param_names;
    param_names.push_back("self");

    setattr("__str__", std::make_shared<PyFunction>(
        __str__, "__str__", param_names
    ));
    setattr("capitalize", std::make_shared<PyFunction>(
        capitalize, "capitalize", param_names
    ));
}

PyObjectPtr PyString::__str__(const ParsedFunctionArguments &args)
{
    return args.get_arg_named("self");
}

PyObjectPtr PyString::capitalize(const ParsedFunctionArguments &args)
{

    std::string new_str = args.get_arg_named("self")->as<PyString>().internal;
    for (char &x : new_str)
        x = toupper(x);
    return helpers::new_string(new_str);
}