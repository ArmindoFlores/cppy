#include "pytype.h"
#include "pyhelpers.h"
#include "pylist.h"
#include "pytraceback.h"
#include "pytypes.h"
using namespace cppy;
#include <algorithm>
#include <unordered_map>

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
                break;
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

    std::vector<PyObjectAnyPtr> anybases = cls->getattr("__bases__")->as<PyList>()->internal;
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
    auto constructor = args.get_arg_named("self")->as<PyType>()->constructor;
    return helpers::call(constructor, args.as_function_args());
}

static PyObjectPtr PyType__repr__(const ParsedFunctionArguments& args)
{
    std::string name = args.get_arg_named("self")->as<PyType>()->name;
    return helpers::new_string("<class '" + name + "'>");
}

static PyObjectPtr PyType_mro(const ParsedFunctionArguments& args)
{
    try {
        return helpers::new_list(mro_r(args.get_arg_named("self"), true));   
    }
    catch (std::invalid_argument&) {
        TB.raise("Cannot create a consistent method resolution", "TypeError");
    }
    // This should never be reached
    return helpers::new_none();
}

PyObjectPtr PyType::__call__ = std::make_shared<PyFunction>(
    PyType__call__, "__call__", std::vector<std::string>({"args", "kwargs"}), 0, true
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

PyObjectPtr PyType::__mro__(const PyObject& self)
{
    const PyType& t = *dynamic_cast<const PyType*>(&self);
    return helpers::new_string(t.name);
}

PyObjectPtr PyType::__name__(const PyObject& self)
{
    const PyType& t = *dynamic_cast<const PyType*>(&self);
    return helpers::new_string(t.name);
}

PyType::PyType(const std::string& name, PyObjectPtr constructor) : name(name), constructor(constructor) 
{
    setattr("__call__", __call__);
    setattr("__repr__", __repr__);
    // setattr("__mro__", __mro__);
    setattr("__name__", __name__);
    setattr("mro", mro);
}
