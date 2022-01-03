#include "pynone.h"
#include "pystring.h"
#include "pyfunction.h"
#include "pytypes.h"
#include "pyhelpers.h"
using namespace cppy;

static PyObjectPtr PyNone__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>("None");
}

static PyObjectPtr PyNone__str__(const ParsedFunctionArguments &args)
{
    return PyNone__repr__(args);
}

PyObjectPtr PyNone::__str__ = std::make_shared<PyFunction>(PyNone__str__, "__str__", std::vector<std::string>({"self"}));
PyObjectPtr PyNone::__repr__ = std::make_shared<PyFunction>(PyNone__repr__, "__repr__", std::vector<std::string>({"self"}));

PyObjectPtr PyNone::__class__(const PyObject&)
{
    return BT.get_type_named("none");
}

PyNone::PyNone()
{
    setattr("__str__", __str__);
    setattr("__repr__", __repr__);
    setattr("__class__", __class__);
}