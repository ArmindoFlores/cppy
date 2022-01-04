#include "pydict.h"
#include "pystring.h"
#include "pyhelpers.h"
#include "pyrecursionctx.h"
#include "pytypes.h"
#include "pytraceback.h"
#include "pyint.h"
using namespace cppy;
#include <sstream>
#include <iostream>

bool CustomCompare::operator()(const PyObjectAnyPtr& ap1, const PyObjectAnyPtr& ap2) const
{
    PyObjectPtr p1, p2;
    if (std::holds_alternative<PyObjectPtr>(ap1))
        p1 = std::get<PyObjectPtr>(ap1);
    else if (std::holds_alternative<PyObjectWPtr>(ap1))
        p1 = std::get<PyObjectWPtr>(ap1).lock();
    if (std::holds_alternative<PyObjectPtr>(ap2))
        p2 = std::get<PyObjectPtr>(ap2);
    else if (std::holds_alternative<PyObjectWPtr>(ap2))
        p2 = std::get<PyObjectWPtr>(ap2).lock();

    long long hash1 = helpers::call_member("__hash__", p1, FunctionArguments({}))->as<PyInt>()->value;
    long long hash2 = helpers::call_member("__hash__", p2, FunctionArguments({}))->as<PyInt>()->value;
    return hash1 < hash2;
}

static PyObjectPtr PyDict__repr__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    RC.enter(self.get());
    std::stringstream ss;

    ss << "{";
    std::size_t i = 0;
    for (auto it = self->as<PyDict>()->internal.begin(); it != self->as<PyDict>()->internal.end(); it++, i++) {
        PyObjectPtr key, value;
        if (std::holds_alternative<PyObjectPtr>(it->first))
            key = std::get<PyObjectPtr>(it->first);
        else if (std::holds_alternative<PyObjectWPtr>(it->first))
            key = std::get<PyObjectWPtr>(it->first).lock();
        else
            return helpers::new_string("");
        if (std::holds_alternative<PyObjectPtr>(it->second))
            value = std::get<PyObjectPtr>(it->second);
        else if (std::holds_alternative<PyObjectWPtr>(it->second))
            value = std::get<PyObjectWPtr>(it->second).lock();
        else
            return helpers::new_string("");

        // Display key
        if (RC.enter(key.get())) {
            auto str = helpers::call_member("__repr__", key, FunctionArguments({}));
            ss << str->as<PyString>()->internal;
            RC.exit(key.get());
        }
        else 
            ss << "{...}";

        ss << ": ";

        // Display value
        if (RC.enter(value.get())) {
            auto str = helpers::call_member("__repr__", value, FunctionArguments({}));
            ss << str->as<PyString>()->internal;
            RC.exit(value.get());
        }
        else 
            ss << "{...}";

        if (i < self->as<PyDict>()->internal.size() - 1) 
            ss << ", ";
    }    
    ss << "}";
    RC.exit(self.get());
    return helpers::new_string(ss.str());
}

static PyObjectPtr PyDict__getitem__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    auto key = args.get_arg_named("y");
    if (!key->hasattr("__hash__") || helpers::is(key->getattr("__hash__"), helpers::new_none())) {
        auto cls = key->getattr("__class__");
        std::string t = cls->getattr("__name__")->as<PyString>()->internal;
        TB.raise("unhashable type: '" + t + "'", "TypeError");
    }

    if (!self->as<PyDict>()->internal.count(key)) {
        std::string r = helpers::call_member("__repr__", key, FunctionArguments({}))->as<PyString>()->internal;
        TB.raise(r, "KeyError");
    }

    auto result = self->as<PyDict>()->internal.at(key);
    if (std::holds_alternative<PyObjectPtr>(result))
        return std::get<PyObjectPtr>(result);
    else if (std::holds_alternative<PyObjectWPtr>(result))
        return std::get<PyObjectWPtr>(result).lock();
    
    // This code should never be reached
    return helpers::new_none();
}

static PyObjectPtr PyDict__setitem__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    auto key = args.get_arg_named("key");
    auto value = args.get_arg_named("value");

    if (!key->hasattr("__hash__") || helpers::is(key->getattr("__hash__"), helpers::new_none())) {
        auto cls = key->getattr("__class__");
        std::string t = cls->getattr("__name__")->as<PyString>()->internal;
        TB.raise("unhashable type: '" + t + "'", "TypeError");
    }

    if (value->gccollected()) {
        if (key->gccollected())
            self->as<PyDict>()->internal[std::weak_ptr(key)] = std::weak_ptr(value);
        else
            self->as<PyDict>()->internal[key] = std::weak_ptr(value);
    }
    else {
        if (key->gccollected())
            self->as<PyDict>()->internal[std::weak_ptr(key)] = value;
        else
            self->as<PyDict>()->internal[key] = value;
    }
    
    return helpers::new_none();
}

static PyObjectPtr PyDict__delitem__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    auto key = args.get_arg_named("key");

    if (!key->hasattr("__hash__") || helpers::is(key->getattr("__hash__"), helpers::new_none())) {
        auto cls = key->getattr("__class__");
        std::string t = cls->getattr("__name__")->as<PyString>()->internal;
        TB.raise("unhashable type: '" + t + "'", "TypeError");
    }
    
    if (!self->as<PyDict>()->internal.count(key)) {
        std::string r = helpers::call_member("__repr__", key, FunctionArguments({}))->as<PyString>()->internal;
        TB.raise(r, "KeyError");
    }

    self->as<PyDict>()->internal.erase(key);

    return helpers::new_none();
}

PyObjectPtr PyDict::__repr__ = std::make_shared<PyFunction>(
    PyDict__repr__,
    "__repr__",
    std::vector<std::string>({"self"})
);

PyObjectPtr PyDict::__delitem__ = std::make_shared<PyFunction>(
    PyDict__delitem__,
    "__delitem__",
    std::vector<std::string>({"self", "key"})
);

PyObjectPtr PyDict::__getitem__ = std::make_shared<PyFunction>(
    PyDict__getitem__,
    "__getitem__",
    std::vector<std::string>({"self", "y"})
);

PyObjectPtr PyDict::__setitem__ = std::make_shared<PyFunction>(
    PyDict__setitem__,
    "__setitem__",
    std::vector<std::string>({"self", "key", "value"})
);

PyObjectPtr PyDict::__class__(const PyObject&)
{
    return BT.get_type_named("dict");
}

void PyDict::construct(PyObject *self)
{
    self->setattr("__repr__", __repr__);
    self->setattr("__delitem__", __delitem__);
    self->setattr("__getitem__", __getitem__);
    self->setattr("__setitem__", __setitem__);
}

PyDict::PyDict()
{
    setattr("__class__", __class__);
}

std::vector<PyObjectWPtr> PyDict::getrefs()
{
    std::vector<PyObjectWPtr> gather;

    for (auto it = internal.begin(); it != internal.end(); it++) {
        if (std::holds_alternative<PyObjectWPtr>(it->first))
            gather.push_back(std::get<PyObjectWPtr>(it->first));
        if (std::holds_alternative<PyObjectWPtr>(it->second))
            gather.push_back(std::get<PyObjectWPtr>(it->second));
    }

    return gather;
}