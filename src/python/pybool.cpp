#include "pybool.h"
#include "pytypes.h"
#include "pyhelpers.h"
using namespace cppy;

static PyObjectPtr PyBool__repr__(const ParsedFunctionArguments &args)
{
    return helpers::new_string(args.get_arg_named("self")->as<PyBool>()->value ? "True" : "False");
}

static PyObjectPtr PyBool__int__(const ParsedFunctionArguments &args)
{
    return helpers::new_int(args.get_arg_named("self")->as<PyBool>()->value);
}

PyObjectPtr PyBool::__repr__ = std::make_shared<PyFunction>(PyBool__repr__, "__repr__", std::vector<std::string>({"self"}));
PyObjectPtr PyBool::__int__ = std::make_shared<PyFunction>(PyBool__int__, "__int__", std::vector<std::string>({"self"}));

PyObjectPtr PyBool::__class__(const PyObject&)
{
    return BT.get_type_named("bool");
}

void PyBool::construct(PyObject *self)
{
    self->setattr("__repr__", __repr__);
    self->setattr("__int__", __int__);
}

PyBool::PyBool()
{
    setattr("__class__", __class__);
}

PyBool::PyBool(bool x) : PyBool()
{
    value = x;
}
