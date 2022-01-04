#include "pystring.h"
#include "pyfunction.h"
#include "pyhelpers.h"
#include "pytraceback.h"
#include "pytypes.h"
using namespace cppy;
#include <iostream>
#include <cctype>
 
static PyObjectPtr PyString__str__(const ParsedFunctionArguments &args)
{
    return args.get_arg_named("self");
}

static PyObjectPtr PyString__repr__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self");
    return helpers::new_string("'" + self->as<PyString>()->internal + "'");
}

static PyObjectPtr PyString_capitalize(const ParsedFunctionArguments &args)
{
    std::string new_str = args.get_arg_named("self")->as<PyString>()->internal;
    for (char &x : new_str)
        x = toupper(x);
    return helpers::new_string(new_str);
}

PyObjectPtr PyString::__str__ = std::make_shared<PyFunction>(
    PyString__str__,
    "__str__",
    std::vector<std::string>({"self"})
);

PyObjectPtr PyString::__repr__ = std::make_shared<PyFunction>(
    PyString__repr__,
    "__repr__",
    std::vector<std::string>({"self"})
);

PyObjectPtr PyString::capitalize = std::make_shared<PyFunction>(
    PyString_capitalize,
    "capitalize",
    std::vector<std::string>({"self"})
);

PyObjectPtr PyString::__class__(const PyObject&)
{
    return BT.get_type_named("str");
}

void PyString::construct(PyObject* self)
{
    self->setattr("__repr__", __repr__);
    self->setattr("__str__", __str__);
    self->setattr("capitalize", capitalize);
}

PyString::PyString()
{
    setattr("__class__", __class__);
}

PyString::PyString(const std::string &s) : PyString()
{
    internal = s;
}