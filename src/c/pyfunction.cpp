#include "pyfunction.h"
#include "pytraceback.h"
using namespace cpy;

FunctionArgument::FunctionArgument(PyObjectPtr obj, const std::string& name) : o(obj), name(name) {}

bool FunctionArgument::is_named() const
{
    return name != "";
}

std::string FunctionArgument::get_name() const 
{
    return name;
}

PyObjectPtr FunctionArgument::get_object() const
{
    return o;
}

PyFunction::PyFunction(std::function<PyObjectPtr(const FunctionArguments&)> f, const std::string& fn) : internal(f), function_name(fn) {}

PyObjectPtr PyFunction::call(const FunctionArguments &args)
{
    cpy::globals::Traceback::the().push(function_name);
    auto result = internal(args);
    cpy::globals::Traceback::the().pop();
    return result;
}