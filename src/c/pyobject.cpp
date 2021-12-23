#include "pyobject.h"
#include "pytraceback.h"
using namespace cpy;

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