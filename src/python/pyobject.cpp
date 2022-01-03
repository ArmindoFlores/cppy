#include "pyobject.h"
#include "pytraceback.h"
#include "pyhelpers.h"
#include "pyfunction.h"
#include "pystring.h"
#include "pylist.h"
using namespace cppy;
#include <sstream>
#include <iostream>

PyObject::PyObject()
{
}

bool PyObject::gccollected() const
{
    return false;
}

std::vector<PyObjectWPtr> PyObject::getrefs()
{
    return std::vector<PyObjectWPtr>();
}

PyObjectPtr PyObject::from_variant(const std::variant<PyObjectPtr, PyObjectWPtr, std::function<PyObjectPtr(const PyObject&)>>& o) const
{
    if (std::holds_alternative<PyObjectPtr>(o))
        return std::get<PyObjectPtr>(o);
    else if (std::holds_alternative<PyObjectWPtr>(o))
        return std::get<PyObjectWPtr>(o).lock();
    else if (std::holds_alternative<std::function<PyObjectPtr(const PyObject&)>>(o))
        return std::get<std::function<PyObjectPtr(const PyObject&)>>(o)(*this);

    // This should never be reached unless we have an invalid variant
    throw std::bad_variant_access();
}

PyObjectPtr PyObject::getattr(const std::string& name) const
{
    if (attributes.count(name))
        return from_variant(attributes.at(name));

    // Prevent recursion (every type object must have its own __mro__ and __class__ attributes)
    if (name == "__class__")
        TB.raise("Fatal: object has no __class__", "Fatal");

    PyObjectPtr cls = from_variant(attributes.at("__class__"));
    std::string cls_name = cls->getattr("__name__")->as<PyString>()->internal;

    if (name == "__mro__")
        TB.raise("'" + cls_name + "' object has no attribute '" + name + "'", "AttributeError");

    auto mro = cls->getattr("__mro__")->as<PyList>()->internal;
    for (auto &type : mro) {
        PyObjectPtr type_ptr;
        if (std::holds_alternative<PyObjectPtr>(type))
            type_ptr = std::get<PyObjectPtr>(type);
        else if (std::holds_alternative<PyObjectWPtr>(type))
            type_ptr = std::get<PyObjectWPtr>(type).lock();
        else 
            continue;

        // Prevent recursion
        if (type_ptr.get() == this)
            continue;

        if (type_ptr->attributes.count(name))
            return from_variant(type_ptr->attributes.at(name));
    }
    TB.raise("'" + cls_name + "' object has no attribute '" + name + "'", "AttributeError");
    
    // This should never be reached
    return helpers::new_none();
}

void PyObject::setattr(const std::string& name, std::function<PyObjectPtr(const PyObject&)> value)
{
    attributes[name] = value;
}

void PyObject::setattr(const std::string& name, PyObjectPtr value)
{
    if (value && !value->gccollected())
        attributes[name] = value;
    else if (value)
        throw std::invalid_argument("PyObject instances can't contain GC objects");
}

void PyObject::setattr(const std::string& name, PyObjectWPtr value)
{
    throw std::invalid_argument("PyObject instances can't contain weak references");
}

bool PyObject::hasattr(const std::string& name) const
{
    if (attributes.count(name))
        return true;

    // Prevent recursion
    if (name == "__class__")
        return false;

    PyObjectPtr cls = from_variant(attributes.at("__class__"));

    auto mro = cls->getattr("__mro__")->as<PyList>()->internal;
    for (auto &type : mro) {
        PyObjectPtr type_ptr;
        if (std::holds_alternative<PyObjectPtr>(type))
            type_ptr = std::get<PyObjectPtr>(type);
        else if (std::holds_alternative<PyObjectWPtr>(type))
            type_ptr = std::get<PyObjectWPtr>(type).lock();
        else 
            continue;

        // Prevent recursion
        if (type_ptr.get() == this)
            continue;

        if (type_ptr->attributes.count(name))
            return true;
    }
    return false;
}

void PyGCObject::setattr(const std::string& name, PyObjectPtr value)
{
    if (value && !value->gccollected())
        attributes[name] = value;
    else if (value)
        attributes[name] = std::weak_ptr(value);
}

void PyGCObject::setattr(const std::string& name, PyObjectWPtr value)
{
    attributes[name] = value;
}

void PyGCObject::setattr(const std::string& name, std::function<PyObjectPtr(const PyObject&)> value)
{
    PyObject::setattr(name, value);
}

bool PyGCObject::gccollected() const
{
    return true;
}

std::vector<PyObjectWPtr> PyUserObject::getrefs()
{
    std::vector<PyObjectWPtr> gather;
    for (auto &attr : attributes) {
        if (std::holds_alternative<PyObjectWPtr>(attr.second) && std::get<PyObjectWPtr>(attr.second).lock()->gccollected())
            gather.push_back(std::get<PyObjectWPtr>(attr.second));
        else if (std::holds_alternative<PyObjectPtr>(attr.second) && std::get<PyObjectPtr>(attr.second)->gccollected())
            gather.push_back(std::weak_ptr(std::get<PyObjectPtr>(attr.second)));
    }
    return gather;
}