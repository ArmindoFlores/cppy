#include "pyobject.h"
#include "pytraceback.h"
#include "pyhelpers.h"
#include "pyfunction.h"
#include <sstream>
#include <iostream>
using namespace cpy;

PyObjectPtr __repr__(const ParsedFunctionArguments& args)
{
    auto memory_location = args.get_arg_named("self").get();
    std::stringstream ss;
    ss << static_cast<const void*>(memory_location);
    return helpers::new_string("<object object at " + ss.str() + ">");
}

PyObjectPtr __str__(const ParsedFunctionArguments& args)
{
    return __repr__(args);
}

static PyObjectPtr object__repr__ = std::make_shared<PyFunction>(
    __repr__, "__repr__", std::vector<std::string>({"self"})
);

static PyObjectPtr object__str__ = std::make_shared<PyFunction>(
    __str__, "__str__", std::vector<std::string>({"self"})
);

PyObject::PyObject()
{
    attributes["__repr__"] = object__repr__;
    attributes["__str__"] = object__str__;
}

PyObjectPtr PyObject::getattr(const std::string& name) const
{
    if (!hasattr(name))
        globals::Traceback::the().raise("AttributeError: X object has no attribute '" + name + "'", "AttributeError");
    return attributes.at(name);
}

void PyObject::setattr(const std::string& name, PyObjectPtr value)
{
    attributes[name] = value;
}

bool PyObject::hasattr(const std::string& name) const
{
    return attributes.count(name);
}