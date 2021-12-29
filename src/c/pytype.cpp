#include "pytype.h"
#include "pyhelpers.h"
using namespace cppy;

PyType::PyType(const std::string& name, PyObjectPtr constructor) : name(name), constructor(constructor) 
{
    setattr("__call__", std::make_shared<PyFunction>(
        __call__, "__call__", std::vector<std::string>({"args", "kwargs"}), 0, true
    ));
    setattr("__str__", std::make_shared<PyFunction>(
        __str__, "__str__", std::vector<std::string>({"self"})
    ));
}

PyObjectPtr PyType::__call__(const ParsedFunctionArguments& args)
{
    auto constructor = args.get_arg_named("self")->as<PyType>()->constructor;
    return helpers::call(constructor, args.as_function_args());
}

PyObjectPtr PyType::__str__(const ParsedFunctionArguments& args)
{
    std::string name = args.get_arg_named("self")->as<PyType>()->name;
    return helpers::new_string("<class '" + name + "'>");
}