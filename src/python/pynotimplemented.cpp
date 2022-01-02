#include "pynotimplemented.h"
#include "pystring.h"
#include "pytypes.h"
using namespace cppy;

PyNotImplemented::PyNotImplemented()
{
    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__", std::vector<std::string>({"self"})));
    setattr("__repr__", std::make_shared<PyFunction>(__repr__, "__repr__", std::vector<std::string>({"self"})));
    setattr("__class__", __class__);
}

PyObjectPtr PyNotImplemented::__str__(const ParsedFunctionArguments &args)
{
    return __repr__(args);
}

PyObjectPtr PyNotImplemented::__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("NotImplemented");
}

PyObjectPtr PyNotImplemented::__class__(const PyObject&)
{
    return BT.get_type_named("notimpl");
}