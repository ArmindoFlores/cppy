#include "pyint.h"
#include "pystring.h"
#include "pytypes.h"
using namespace cpy;

PyInt::PyInt()
{
    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__", std::vector<std::string>({"self"})));
    setattr("__repr__", std::make_shared<PyFunction>(__repr__, "__repr__", std::vector<std::string>({"self"})));
    setattr("__class__", __class__);
}

PyInt::PyInt(long long x) : PyInt()
{
    value = x;
}

PyObjectPtr PyInt::__str__(const ParsedFunctionArguments &args)
{
    return __repr__(args);
}

PyObjectPtr PyInt::__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>(std::to_string(args.get_arg_named("self")->as<PyInt>().value));
}

PyObjectPtr PyInt::__class__(const PyObject&)
{
    return BT.get_type_named("int");
}