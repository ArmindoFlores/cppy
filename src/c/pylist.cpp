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

PyList::PyList() 
{
    setattr("__repr__", std::make_shared<PyFunction>(
        __repr__,
        "__repr__",
        std::vector<std::string>({"self"})
    ));
    setattr("__class__", __class__);
    setattr("append", std::make_shared<PyFunction>(
        append,
        "append",
        std::vector<std::string>({"self", "object"})
    ));
    setattr("pop", std::make_shared<PyFunction>(
        pop,
        "pop",
        std::vector<std::string>({"self", "i"}),
        std::vector<PyObjectPtr>({helpers::new_int(-1)}),
        1
    ));
}

PyObjectPtr PyList::__repr__(const ParsedFunctionArguments& args)
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

PyObjectPtr PyList::__class__(const PyObject&)
{
    return BT.get_type_named("list");
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

PyObjectPtr PyList::append(const ParsedFunctionArguments& args)
{    
    auto self = args.get_arg_named("self");
    auto obj = args.get_arg_named("object");

    if (self->gccollected())
        self->as<PyList>()->internal.push_back(std::weak_ptr(obj));
    else
        self->as<PyList>()->internal.push_back(obj);

    return helpers::new_none();
}

PyObjectPtr PyList::pop(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self")->as<PyList>();
    auto index_obj = args.get_arg_named("i")->as<PyInt>();
    long long index = index_obj->value >= 0 ? index_obj->value : self->internal.size() + index_obj->value;

    // FIXME: allow for pop to operate on any element
    if (index != self->internal.size()-1) {
        TB.raise("index != -1", "NotImplementedError");
    }

    auto result = self->internal.back();
    self->internal.pop_back();

    if (std::holds_alternative<PyObjectPtr>(result))
        return std::get<PyObjectPtr>(result);
    else if (std::holds_alternative<PyObjectWPtr>(result))
        return std::get<PyObjectWPtr>(result).lock();
    return helpers::new_none();
}