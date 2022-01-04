#include "pytype.h"
#include "pyhelpers.h"
#include "pylist.h"
#include "pytraceback.h"
#include "pytuple.h"
#include "pytypes.h"
#include "pystring.h"
#include "pydict.h"
#include "pygarbagecollector.h"
using namespace cppy;
#include <algorithm>
#include <unordered_map>
#include <iostream>

/*
 * Merge function for the default MRO function
 */
std::vector<PyObjectPtr> mro_merge(std::vector<std::vector<PyObjectPtr>>& lists)
{
    std::vector<PyObjectPtr> result;
    bool stop = false;
    do {
        PyObjectPtr ptr;
        bool found = false;
        stop = true;

        // Iterate through the first element of every list
        for (auto &list : lists) {
            if (list.size() == 0)
                continue;

            // If we got here it means there is still more to compute
            stop = false;
            auto &candidate = list[0];

            // Assume candidate is valid
            bool valid = true;
            for (auto &list2 : lists) {
                auto index = std::find(list2.begin(), list2.end(), candidate);
                if (index != list2.end() && (std::size_t)(index - list2.begin()) != 0) {
                    // The candidate is present in another list and is not its head
                    valid = false;
                    break;
                }
            }
            if (valid) {
                // We found a valid candidate to merge
                found = true;
                ptr = candidate;
                //break;
            }
        }
        // If we tried to compute somethind
        if (!stop) {
            if (found) {
                // We found a way to merge, let's remove the candidate
                // from all the lists and add it to the result
                result.push_back(ptr);
                for (auto &list : lists) {
                    for (auto it = list.begin(); it != list.end(); ) {
                        if (*it == ptr)
                            it = list.erase(it);
                        else
                            it++;
                    }
                }
            }
            else {
                // There's no way to merge
                throw std::invalid_argument("Unmergeable");
            }
        }
    } while (!stop);
    return result;
}

/*
 * Default algorithm for determining the MRO (method resolution order)
 * This is described here (https://www.python.org/download/releases/2.3/mro/)
 */
static std::vector<PyObjectPtr> mro_r(PyObjectPtr cls, bool reset=false)
{
    // Cache results to prevent infinite recursion
    static std::unordered_map<PyObjectPtr, std::vector<PyObjectPtr>> cache;
    if (reset)
        cache.clear();
    else if (cache.count(cls))
        return cache.at(cls);
    
    std::vector<PyObjectPtr> result;

    std::vector<PyObjectAnyPtr> anybases = cls->getattr("__bases__")->as<PyTuple>()->internal;
    std::vector<PyObjectPtr> bases(anybases.size());
    for (std::size_t i = 0; i < anybases.size(); i++) {
        if (std::holds_alternative<PyObjectPtr>(anybases[i]))
            bases[i] = std::get<PyObjectPtr>(anybases[i]);
        else if (std::holds_alternative<PyObjectWPtr>(anybases[i]))
            bases[i] = std::get<PyObjectWPtr>(anybases[i]).lock();
        else {
            // This should never happen
            throw std::bad_variant_access();
        }
    }
    anybases.clear();
    std::vector<std::vector<PyObjectPtr>> args(bases.size() + 1);
    for (std::size_t i = 0; i < bases.size(); i++)
        args[i] = mro_r(bases[i]);
    args[args.size()-1] = bases;

    for (auto &element : mro_merge(args))
        result.push_back(element);

    cache[cls] = result;
    return result;
}

static PyObjectPtr PyType__call__(const ParsedFunctionArguments& args)
{
    // FIXME: do something here
    return helpers::new_none();
}

static PyObjectPtr PyType__init__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    self->setattr("__mro__", helpers::new_tuple(mro_r(args.get_arg_named("self"), true)));
    // auto &unnamed_args = args.get_args();

    // if (unnamed_args.size() == 1) {
    //     // I don't know what this does
    //     return helpers::new_none();
    // }
    // if (unnamed_args.size() == 3) {
    //     // I don't know what this does
    //     return helpers::new_none();
    // }
    // TB.raise("type.__init__() takes 1 or 3 arguments", "TypeError");
    // This should never be reached
    return helpers::new_none();
}

static PyObjectPtr PyType__repr__(const ParsedFunctionArguments& args)
{
    std::string name = args.get_arg_named("self")->getattr("__name__")->as<PyString>()->internal;
    return helpers::new_string("<class '" + name + "'>");
}

static PyObjectPtr PyType_mro(const ParsedFunctionArguments& args)
{
    try {
        return helpers::new_tuple(mro_r(args.get_arg_named("self"), true));   
    }
    catch (std::invalid_argument&) {
        TB.raise("Cannot create a consistent method resolution", "TypeError");
    }
    // This should never be reached
    return helpers::new_none();
}

static PyObjectPtr PyType__new__(const ParsedFunctionArguments& args)
{
    //FIXME: this is only correct for when unnamed_args[0] == <class 'type'>
    auto &unnamed_args = args.get_args();

    if (unnamed_args.size() == 2) {
        // Returns the type of an object
        if (unnamed_args[0] != BT.get_type_named("type")) {
            TB.raise("type.__new__() takes exactly 3 arguments (1 given)", "TypeError");
        }
        return unnamed_args[1]->getattr("__class__");
    }
    if (unnamed_args.size() == 4) {
        // FIXME: this is very likely wrong
        auto new_type = std::make_shared<PyUserObject>();
        // new_type->setattr("__new__", unnamed_args[0]->getattr("__new__"));
        new_type->setattr("__class__", unnamed_args[0]);
        new_type->setattr("__bases__", unnamed_args[2]);
        new_type->setattr("__name__", unnamed_args[1]);
        new_type->setattr("__mro__", helpers::call(
            unnamed_args[0]->getattr("mro"),
            FunctionArguments({std::dynamic_pointer_cast<PyObject>(new_type)})
        ));
        GC.add_container(std::dynamic_pointer_cast<PyObject>(new_type));
        return new_type;
    }
    TB.raise("type.__init__() takes 1 or 3 arguments", "TypeError");
    // This should never be reached
    return helpers::new_none();
}

PyObjectPtr PyType::__call__ = std::make_shared<PyFunction>(
    PyType__call__, "__call__", std::vector<std::string>({"args", "kwargs"}), 0, true
);

PyObjectPtr PyType::__init__ = std::make_shared<PyFunction>(
    PyType__init__, "__init__", std::vector<std::string>({"self", "args", "kwargs"}), 1, true
);

PyObjectPtr PyType::__new__ = std::make_shared<PyFunction>(
    PyType__new__, "__new__", std::vector<std::string>({"args", "kwargs"}), 0, true
);

PyObjectPtr PyType::__repr__ = std::make_shared<PyFunction>(
    PyType__repr__, "__repr__", std::vector<std::string>({"self"})
);

PyObjectPtr PyType::mro = std::make_shared<PyFunction>(
    PyType_mro, "mro", std::vector<std::string>({"self"})
);

PyObjectPtr PyType::__class__(const PyObject& self)
{
    return BT.get_type_named("type");
}

PyObjectPtr PyType::__name__(const PyObject& self)
{
    const PyType& t = *dynamic_cast<const PyType*>(&self);
    return helpers::new_string(t.name);
}

void PyType::construct(PyObject* self)
{
    self->setattr("__call__", __call__);
    self->setattr("__class__", __class__);
    self->setattr("__init__", __init__);
    self->setattr("__name__", __name__);
    self->setattr("__new__", __new__);
    self->setattr("__repr__", __repr__);
    self->setattr("mro", mro);
}

PyType::PyType(const std::string& name, std::function<void(PyObject*)> constructor, PyObjectPtr bases) : name(name) 
{
    setattr("__bases__", bases);
    setattr("__class__", __class__);
    setattr("__name__", __name__);
    constructor(this);
}
