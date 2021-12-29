#include "pyhelpers.h"
#include "pystring.h"
#include "pyint.h"
#include "pygarbagecollector.h"
#include "pylist.h"
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

PyObjectPtr helpers::new_list()
{
    PyObjectPtr l = std::make_shared<PyList>();
    GC.add_container(l);
    return l;
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
    return call(obj->getattr(name), new_args);
}