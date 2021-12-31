#include "pytype.h"
#include "pyhelpers.h"
using namespace cppy;

static PyObjectPtr PyType__call__(const ParsedFunctionArguments& args)
{
    auto constructor = args.get_arg_named("self")->as<PyType>()->constructor;
    return helpers::call(constructor, args.as_function_args());
}

static PyObjectPtr PyType__repr__(const ParsedFunctionArguments& args)
{
    std::string name = args.get_arg_named("self")->as<PyType>()->name;
    return helpers::new_string("<class '" + name + "'>");
}

PyObjectPtr PyType::__call__ = std::make_shared<PyFunction>(
    PyType__call__, "__call__", std::vector<std::string>({"args", "kwargs"}), 0, true
);

PyObjectPtr PyType::__repr__ = std::make_shared<PyFunction>(
    PyType__repr__, "__repr__", std::vector<std::string>({"self"})
);

PyObjectPtr PyType::__name__(const PyObject& self)
{
    const PyType& t = *dynamic_cast<const PyType*>(&self);
    return helpers::new_string(t.name);
}

PyType::PyType(const std::string& name, PyObjectPtr constructor) : name(name), constructor(constructor) 
{
    setattr("__call__", __call__);
    setattr("__repr__", __repr__);
    setattr("__name__", __name__);
}
