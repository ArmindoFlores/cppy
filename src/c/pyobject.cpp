#include "pyobject.h"
#include "pytraceback.h"
#include "pyhelpers.h"
#include "pyfunction.h"
#include <sstream>
#include <iostream>
using namespace cppy;

PyObjectPtr __repr__(const ParsedFunctionArguments& args)
{
    auto memory_location = args.get_arg_named("self").get();
    std::stringstream ss;
    ss << static_cast<const void*>(memory_location);
    return helpers::new_string("<object object at " + ss.str() + ">");
}

PyObjectPtr __str__(const ParsedFunctionArguments& args)
{
    return helpers::call_member("__repr__", args.get_arg_named("self"), FunctionArguments({}));
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

bool PyObject::gccollected()
{
    return false;
}

std::vector<PyObjectWPtr> PyObject::getrefs()
{
    return std::vector<PyObjectWPtr>();
}

PyObjectPtr PyObject::getattr(const std::string& name) const
{
    if (!hasattr(name))
        TB.raise("X object has no attribute '" + name + "'", "AttributeError");

    auto &attr = attributes.at(name);

    if (std::holds_alternative<PyObjectPtr>(attr))
        return std::get<PyObjectPtr>(attr);
    else if (std::holds_alternative<std::function<PyObjectPtr(const PyObject&)>>(attr))
        return std::get<std::function<PyObjectPtr(const PyObject&)>>(attr)(*this);
    
    // This should never be reached unless we have an invalid variant
    throw std::bad_variant_access();
}

void PyObject::setattr(const std::string& name, PyObjectPtrOrFunc value)
{
    attributes[name] = value;
}

bool PyObject::hasattr(const std::string& name) const
{
    return attributes.count(name);
}