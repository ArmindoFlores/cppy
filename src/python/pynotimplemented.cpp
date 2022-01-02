#include "pynotimplemented.h"
#include "pystring.h"
#include "pytypes.h"
using namespace cppy;

static PyObjectPtr PyNotImplemented__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("NotImplemented");
}

static PyObjectPtr PyNotImplemented__str__(const ParsedFunctionArguments &args)
{
    return PyNotImplemented__repr__(args);
}

PyObjectPtr PyNotImplemented::__repr__ = std::make_shared<PyFunction>(PyNotImplemented__repr__, "__repr__", std::vector<std::string>({"self"}));
PyObjectPtr PyNotImplemented::__str__ = std::make_shared<PyFunction>(PyNotImplemented__str__, "__str__", std::vector<std::string>({"self"}));

PyObjectPtr PyNotImplemented::__class__(const PyObject&)
{
    return BT.get_type_named("notimpl");
}

PyNotImplemented::PyNotImplemented()
{
    setattr("__str__", __str__);
    setattr("__repr__", __repr__);
    setattr("__class__", __class__);
}