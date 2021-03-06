#include "pyhelpers.h"
#include "pystring.h"
#include "pyint.h"
#include "pygarbagecollector.h"
#include "pylist.h"
#include "pytuple.h"
#include "pybool.h"
#include "pydict.h"
#include "pyfunction.h"
#include "pytraceback.h"
#include "pyglobalinstances.h"
using namespace cppy;

PyObjectPtr helpers::new_string(const std::string& s)
{
    return std::make_shared<PyString>(s);
}

PyObjectPtr helpers::new_int(long long x)
{
    return std::make_shared<PyInt>(x);
}

PyObjectPtr helpers::new_none()
{
    // Note: this doesn't actually create a new object
    return GI.get("none");
}

PyObjectPtr helpers::new_bool(bool x)
{
    // Note: this doesn't actually create a new object
    if (x) 
        return GI.get("True");
    return GI.get("False");
}

PyObjectPtr helpers::new_notimpl()
{
    // Note: this doesn't actually create a new object
    return GI.get("notimpl");
}

PyObjectPtr helpers::new_tuple(const std::vector<PyObjectAnyPtr>& contents)
{
    PyObjectPtr t = std::make_shared<PyTuple>(contents);
    if (t->gccollected()) GC.add_container(t);
    return t;
}

PyObjectPtr helpers::new_tuple()
{
    return new_tuple(std::vector<PyObjectAnyPtr>({}));
}

PyObjectPtr helpers::new_tuple(const std::vector<PyObjectPtr>& contents)
{
    std::vector<PyObjectAnyPtr> new_contents(contents.size());
    for (std::size_t i = 0; i < contents.size(); i++)
        new_contents[i] = contents[i];
    return new_tuple(new_contents);
}

PyObjectPtr helpers::new_list()
{
    PyObjectPtr l = std::make_shared<PyList>();
    GC.add_container(l);
    return l;
}

PyObjectPtr helpers::new_dict()
{
    PyObjectPtr d = std::make_shared<PyDict>();
    GC.add_container(d);
    return d;
}

PyObjectPtr helpers::new_list(const std::vector<PyObjectPtr>& objs)
{
    // FIXME: add a constructor to PyList for this
    PyObjectPtr l = std::make_shared<PyList>();
    for (const auto& obj : objs)
        l->as<PyList>()->internal.push_back(obj);
    GC.add_container(l);
    return l;
}

PyObjectPtr helpers::call(PyObjectPtr callable, FunctionArguments args)
{
    // First, check if the callable's type is known
    auto pyfunc_test = dynamic_cast<PyFunction*>(callable.get());
    if (pyfunc_test != nullptr) {
        return pyfunc_test->call(args);
    }

    // Otherwise, just use the __call__ method
    auto f = callable->getattr("__call__");
    if (f == nullptr) 
       TB.raise("object is not callable", "TypeError");
    return f->as<PyFunction>()->call(args);
}

PyObjectPtr helpers::call_member(const std::string& name, PyObjectPtr obj, FunctionArguments args)
{
    // FIXME: verify if the function is a static member
    FunctionArguments new_args(args.size()+1);
    new_args[0] = obj;
    std::copy(args.begin(), args.end(), new_args.begin()+1);
    return call(obj->getattr("__class__")->getattr(name), new_args);
}

inline bool helpers::is(PyObjectPtr obj1, PyObjectPtr obj2)
{
    return obj1 == obj2;
}

bool helpers::cbool(PyObjectPtr obj)
{
    if (!obj->hasattr("__bool__"))
        return true;
    return call_member("__bool__", obj, FunctionArguments({}))->as<PyBool>()->value;
}

PyObjectPtr helpers::add(PyObjectPtr left, PyObjectPtr right)
{
    if (left->hasattr("__add__")) {
        PyObjectPtr result = call_member("__add__", left, FunctionArguments({right}));
        if (!is(result, helpers::new_notimpl())) {
            return result;
        }
    }
    if (right->hasattr("__radd__")) {
        PyObjectPtr result = call_member("__radd__", right, FunctionArguments({left}));
        if (is(result, helpers::new_notimpl())) {
            TB.raise("unsupported operand type(s) for +: 'X' and 'X'", "TypeError");
        }
        return result;
    }
    TB.raise("unsupported operand type(s) for +: 'X' and 'X'", "TypeError");
    // This should never be reached
    return helpers::new_none();
}

PyObjectPtr helpers::sub(PyObjectPtr left, PyObjectPtr right)
{
    if (left->hasattr("__sub__")) {
        PyObjectPtr result = call_member("__sub__", left, FunctionArguments({right}));
        if (!is(result, helpers::new_notimpl())) {
            return result;
        }
    }
    if (right->hasattr("__rsub__")) {
        PyObjectPtr result = call_member("__rsub__", right, FunctionArguments({left}));
        if (is(result, helpers::new_notimpl())) {
            TB.raise("unsupported operand type(s) for +: 'X' and 'X'", "TypeError");
        }
        return result;
    }
    TB.raise("unsupported operand type(s) for -: 'X' and 'X'", "TypeError");
    // This should never be reached
    return helpers::new_none();
}