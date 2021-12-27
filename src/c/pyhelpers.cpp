#include "pyhelpers.h"
#include "pystring.h"
#include "pyint.h"
#include "pyfunction.h"
#include "pytraceback.h"
#include "pyglobalinstances.h"
using namespace cpy;

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
       TB.raise("TypeError: object is not callable", "TypeError");
    return f->as<PyFunction>().call(args);
}

PyObjectPtr helpers::call_member(const std::string& name, PyObjectPtr obj, FunctionArguments args)
{
    // FIXME: verify if the function is a static member
    FunctionArguments new_args(args.size()+1);
    new_args[0] = obj;
    std::copy(args.begin(), args.end(), new_args.begin());
    return call(obj->getattr(name), new_args);
}