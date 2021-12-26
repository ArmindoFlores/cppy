#include "pynone.h"
#include "pystring.h"
#include "pyfunction.h"
#include "pytypes.h"
using namespace cpy;

PyNone::PyNone()
{
    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__", std::vector<std::string>({"self"})));
    setattr("__repr__", std::make_shared<PyFunction>(__repr__, "__repr__", std::vector<std::string>({"self"})));
    setattr("__class__", __class__);
}

PyObjectPtr PyNone::__str__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}

PyObjectPtr PyNone::__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}

PyObjectPtr PyNone::__class__()
{
    return BT.get_type_named("none");
}