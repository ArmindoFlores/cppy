#include "pylist.h"
#include "pystring.h"
#include "pygarbagecollector.h"
#include "pyint.h"
#include "pyhelpers.h"
#include "pyrecursionctx.h"
#include "pytraceback.h"
#include "pytypes.h"
using namespace cppy;
#include <sstream>

static PyObjectPtr PyList__repr__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    RC.enter(self.get());
    std::stringstream ss;
    ss << "[";
    for (std::size_t i = 0; i < self->as<PyList>()->internal.size(); i++) {
        PyObjectPtr obj;
        if (std::holds_alternative<PyObjectPtr>(self->as<PyList>()->internal[i]))
            obj = std::get<PyObjectPtr>(self->as<PyList>()->internal[i]);
        else if (std::holds_alternative<PyObjectWPtr>(self->as<PyList>()->internal[i]))
            obj = std::get<PyObjectWPtr>(self->as<PyList>()->internal[i]).lock();
        else
            return helpers::new_string("");

        if (RC.enter(obj.get())) {
            auto str = helpers::call_member("__repr__", obj, FunctionArguments({}));
            ss << str->as<PyString>()->internal;
            RC.exit(obj.get());
        }
        else 
            ss << "[...]";
        if (i != self->as<PyList>()->internal.size() - 1) 
            ss << ", ";
    }    
    ss << "]";
    RC.exit(self.get());
    return helpers::new_string(ss.str());
}

static PyObjectPtr PyList_append(const ParsedFunctionArguments& args)
{    
    auto self = args.get_arg_named("self");
    auto obj = args.get_arg_named("object");

    if (obj->gccollected())
        self->as<PyList>()->internal.push_back(std::weak_ptr(obj));
    else
        self->as<PyList>()->internal.push_back(obj);

    return helpers::new_none();
}

static PyObjectPtr PyList_pop(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self")->as<PyList>();
    auto index_obj = args.get_arg_named("i")->as<PyInt>();
    long long index = index_obj->value >= 0 ? index_obj->value : self->internal.size() + index_obj->value;

    if (index >= self->internal.size() || index < 0) {
        TB.raise("list index out of range", "IndexError");
    }

    auto result = self->internal[index];

    self->internal.erase(self->internal.begin() + index);

    if (std::holds_alternative<PyObjectPtr>(result))
        return std::get<PyObjectPtr>(result);
    else if (std::holds_alternative<PyObjectWPtr>(result))
        return std::get<PyObjectWPtr>(result).lock();
    return helpers::new_none();
}

PyObjectPtr PyList::__repr__ = std::make_shared<PyFunction>(
    PyList__repr__,
    "__repr__",
    std::vector<std::string>({"self"})
);

PyObjectPtr PyList::append = std::make_shared<PyFunction>(
    PyList_append,
    "append",
    std::vector<std::string>({"self", "object"})
);

PyObjectPtr PyList::pop = std::make_shared<PyFunction>(
    PyList_pop,
    "pop",
    std::vector<std::string>({"self", "i"}),
    std::vector<PyObjectPtr>({helpers::new_int(-1)}),
    1
);

PyObjectPtr PyList::__class__(const PyObject&)
{
    return BT.get_type_named("list");
}

PyObjectPtr PyList::__hash__(const PyObject&)
{
    return helpers::new_none();
}

PyList::PyList() 
{
    setattr("__repr__", __repr__);
    setattr("__class__", __class__);
    setattr("__hash__", __hash__);
    setattr("append", append);
    setattr("pop", pop);
}

std::vector<PyObjectWPtr> PyList::getrefs()
{
    std::vector<PyObjectWPtr> refs;
    for (auto &elem : internal) {
        if (std::holds_alternative<PyObjectWPtr>(elem))
            refs.push_back(std::get<PyObjectWPtr>(elem));
    }
    return refs;
}

bool PyList::gccollected()
{
    return true;
}
