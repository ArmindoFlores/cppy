#include "pytypes.h"
#include "pyhelpers.h"
#include "pybuiltins.h"
#include "pyint.h"
#include "pybaseobject.h"
#include "pylist.h"
#include "pydict.h"
#include "pytuple.h"
#include "pynotimplemented.h"
#include "pystring.h"
#include "pygarbagecollector.h"
#include "pyglobalinstances.h"
#include "pytraceback.h"
using namespace cppy;
using namespace cppy::globals;
#include <iostream>

std::shared_ptr<BuiltinTypes> BuiltinTypes::instance = nullptr;

BuiltinTypes::BuiltinTypes()
{
    // <class 'object'>
    types["object"] = std::make_shared<PyType>(
        "object",
        PyBaseObject::construct,
        helpers::new_tuple()
    );

    // <class 'type'>
    types["type"] = std::make_shared<PyType>(
        "type",
        PyType::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'function'>
    types["function"] = std::make_shared<PyType>(
        "function",
        PyFunction::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'NoneType'>
    types["none"] = std::make_shared<PyType>(
        "NoneType",
        PyNone::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'NotImplemented'>
    types["notimpl"] = std::make_shared<PyType>(
        "NotImplemented",
        PyNotImplemented::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'str'>
    types["str"] = std::make_shared<PyType>(
        "str",
        PyString::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'int'>
    types["int"] = std::make_shared<PyType>(
        "int",
        PyInt::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'list'>
    types["list"] = std::make_shared<PyType>(
        "list",
        PyList::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'tuple'>
    types["tuple"] = std::make_shared<PyType>(
        "tuple",
        PyTuple::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    // <class 'dict'>
    types["dict"] = std::make_shared<PyType>(
        "dict",
        PyDict::construct,
        helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(types["object"])}))
    );

    init_builtin_types();
}

BuiltinTypes &BuiltinTypes::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<BuiltinTypes>(new BuiltinTypes());
    }
    return *instance;
}

PyObjectPtr BuiltinTypes::get_type_named(const std::string& name) const
{
    return types.at(name);
}

bool BuiltinTypes::is_builtin_type(PyObjectPtr t) const
{
    for (const auto &pair: types) {
        if (pair.second == t)
            return true;
    }
    return false;
}

void BuiltinTypes::init_builtin_types()
{
    // Hardcode MROs for the builtin types
    for (auto &pair : types) {
        if (pair.first != "object")
            pair.second->setattr("__mro__", helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(pair.second), std::weak_ptr(types["object"])})));
        else
            pair.second->setattr("__mro__", helpers::new_tuple(std::vector<PyObjectAnyPtr>({std::weak_ptr(pair.second)})));
    }
}
