#include "pystring.h"
#include "pyfunction.h"
#include "pyhelpers.h"
#include "pytraceback.h"
#include "pytypes.h"
using namespace cpy;
#include <iostream>
#include <cctype>

PyString::PyString()
{
    setattr("__str__", std::make_shared<PyFunction>(
        __str__, "__str__", std::vector<std::string>({"self"})
    ));
    setattr("__class__", __class__);
    setattr("capitalize", std::make_shared<PyFunction>(
        capitalize, "capitalize", std::vector<std::string>({"self"})
    ));
}

PyString::PyString(const std::string &s) : PyString()
{
    internal = s;
}

PyObjectPtr PyString::__str__(const ParsedFunctionArguments &args)
{
    return args.get_arg_named("self");
}

PyObjectPtr PyString::__class__()
{
    return BT.get_type_named("str");
}

PyObjectPtr PyString::capitalize(const ParsedFunctionArguments &args)
{
    std::string new_str = args.get_arg_named("self")->as<PyString>().internal;
    for (char &x : new_str)
        x = toupper(x);
    return helpers::new_string(new_str);
}