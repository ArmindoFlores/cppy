#include "pytuple.h"
#include "pystring.h"
#include "pygarbagecollector.h"
#include "pyint.h"
#include "pyhelpers.h"
#include "pyrecursionctx.h"
#include "pytraceback.h"
#include "pytypes.h"
using namespace cppy;
#include <sstream>

static PyObjectPtr PyTuple__repr__(const ParsedFunctionArguments& args)
{
    auto self = args.get_arg_named("self");
    RC.enter(self.get());
    std::stringstream ss;
    ss << "(";
    for (std::size_t i = 0; i < self->as<PyTuple>()->internal.size(); i++) {
        PyObjectPtr obj;
        if (std::holds_alternative<PyObjectPtr>(self->as<PyTuple>()->internal[i]))
            obj = std::get<PyObjectPtr>(self->as<PyTuple>()->internal[i]);
        else if (std::holds_alternative<PyObjectWPtr>(self->as<PyTuple>()->internal[i]))
            obj = std::get<PyObjectWPtr>(self->as<PyTuple>()->internal[i]).lock();
        else
            return helpers::new_string("");

        if (RC.enter(obj.get())) {
            auto str = helpers::call_member("__repr__", obj, FunctionArguments({}));
            ss << str->as<PyString>()->internal;
            RC.exit(obj.get());
        }
        else 
            ss << "(...)";
        ss << ", ";
    }    
    RC.exit(self.get());
    ss << ")";
    return helpers::new_string(ss.str());
}

PyObjectPtr PyTuple::__repr__ = std::make_shared<PyFunction>(
    PyTuple__repr__,
    "__repr__",
    std::vector<std::string>({"self"})
);

PyObjectPtr PyTuple::__class__(const PyObject&)
{
    return BT.get_type_named("tuple");
}

PyObjectPtr PyTuple::__hash__(const PyObject&)
{
    return helpers::new_none();
}

PyTuple::PyTuple(const std::vector<PyObjectAnyPtr>& i)
{
    for (auto &element : i) {
        if (std::holds_alternative<PyObjectPtr>(element)) {
            PyObjectPtr ptr = std::get<PyObjectPtr>(element);
            if (ptr->gccollected())
                internal.push_back(std::weak_ptr(ptr));
            else
                internal.push_back(ptr);
        }
        else if (std::holds_alternative<PyObjectWPtr>(element))
            internal.push_back(std::get<PyObjectWPtr>(element));
        else 
            continue;
    }
    setattr("__class__", __class__);
}

void PyTuple::construct(PyObject *self)
{
    self->setattr("__repr__", __repr__);
    self->setattr("__hash__", __hash__);
}

std::vector<PyObjectWPtr> PyTuple::getrefs()
{
    std::vector<PyObjectWPtr> refs;
    for (auto &elem : internal) {
        if (std::holds_alternative<PyObjectWPtr>(elem))
            refs.push_back(std::get<PyObjectWPtr>(elem));
    }
    return refs;
}

bool PyTuple::gccollected() const 
{
    for (auto &elem : internal) {
        if (std::holds_alternative<PyObjectWPtr>(elem)) {
            if (std::get<PyObjectWPtr>(elem).lock()->gccollected())
                return true;
        }
        else if (std::holds_alternative<PyObjectPtr>(elem)) {
            if (std::get<PyObjectPtr>(elem)->gccollected())
                return true;
        }
    } 
    return false;
}