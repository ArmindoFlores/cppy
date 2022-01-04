#include "pybaseobject.h"
#include "pyfunction.h"
#include "pyhelpers.h"
#include "pytypes.h"
using namespace cppy;
#include <sstream>

PyObjectPtr PyBaseObject__repr__(const ParsedFunctionArguments& args)
{
    auto memory_location = args.get_arg_named("self").get();
    std::stringstream ss;
    ss << static_cast<const void*>(memory_location);
    return helpers::new_string("<object object at " + ss.str() + ">");
}

PyObjectPtr PyBaseObject__str__(const ParsedFunctionArguments& args)
{
    return helpers::call_member("__repr__", args.get_arg_named("self"), FunctionArguments({}));
}

PyObjectPtr PyBaseObject__hash__(const ParsedFunctionArguments& args)
{
    return helpers::new_int(reinterpret_cast<std::intptr_t>(args.get_arg_named("self").get()));
}

PyObjectPtr PyBaseObject__init__(const ParsedFunctionArguments& args)
{
    return helpers::new_none();
}

PyObjectPtr PyBaseObject::__repr__ = std::make_shared<PyFunction>(
    PyBaseObject__repr__, "__repr__", std::vector<std::string>({"self"})
);

PyObjectPtr PyBaseObject::__str__ = std::make_shared<PyFunction>(
    PyBaseObject__str__, "__str__", std::vector<std::string>({"self"})
);

PyObjectPtr PyBaseObject::__hash__ = std::make_shared<PyFunction>(
    PyBaseObject__hash__, "__hash__", std::vector<std::string>({"self"})
);

PyObjectPtr PyBaseObject::__init__ = std::make_shared<PyFunction>(
    PyBaseObject__init__, "__init__", std::vector<std::string>({"self", "args", "kwargs"}), 1, true
);

PyObjectPtr PyBaseObject::__class__(const PyObject&)
{
    return BT.get_type_named("object");
}

void PyBaseObject::construct(PyObject* self)
{
    self->setattr("__repr__", __repr__);
    self->setattr("__str__", __str__);
    self->setattr("__hash__", __hash__);
    self->setattr("__init__", __init__);
}

PyBaseObject::PyBaseObject()
{
    setattr("__class__", __class__);
}