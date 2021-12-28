#include <iostream>
#include "pybuiltins.h"
#include "pyfunction.h"
#include "pystring.h"
#include "pyint.h"
#include "pytypes.h"
#include "pytraceback.h"
#include "pyhelpers.h"
using namespace cppy;
using namespace cppy::globals;

std::shared_ptr<BuiltinFunctions> BuiltinFunctions::instance = nullptr;

BuiltinFunctions::BuiltinFunctions()
{
    // <built-in function print>
    functions["print"] = std::make_shared<PyFunction>(
        __print__,
        "print",
        std::vector<std::string>({"values", "sep", "end"}),
        std::vector<PyObjectPtr>({helpers::new_string(" "), helpers::new_string("\n")}),
        1,
        0
    );

    // <built-in function id>
    functions["id"] = std::make_shared<PyFunction>(
        __id__,
        "id",
        std::vector<std::string>({"obj"})
    );
}

BuiltinFunctions &BuiltinFunctions::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<BuiltinFunctions>(new BuiltinFunctions());
    }
    return *instance;
}

PyObjectPtr BuiltinFunctions::__print__(const ParsedFunctionArguments& args)
{
    auto &star_args = args.get_args();
    for (std::size_t i = 0; i < star_args.size(); i++) {
        const auto& arg = star_args[i];

        PyObjectPtr obj_result = helpers::call_member("__str__", arg, FunctionArguments());
        PyString *obj_string = dynamic_cast<PyString*>(obj_result.get());
        if (obj_string == nullptr) TB.raise("TypeError: __str__() must return a string", "TypeError");

        // FIXME: sep must already be a string or None
        PyObjectPtr sep_result = helpers::call_member("__str__", args.get_arg_named("sep"), FunctionArguments());
        PyString *sep_string = dynamic_cast<PyString*>(sep_result.get());
        if (sep_string == nullptr) TB.raise("TypeError: __str__() must return a string", "TypeError");

        std::cout << obj_string->internal;
        if (i != star_args.size()-1) 
            std::cout << sep_string->internal;
    }

    // FIXME: end must already be a string or None
    PyObjectPtr result = helpers::call_member("__str__", args.get_arg_named("end"), FunctionArguments());
    PyString *end_string = dynamic_cast<PyString*>(result.get());
    if (end_string == nullptr) TB.raise("TypeError: __str__() must return a string", "TypeError");

    std::cout << end_string->internal;
    return helpers::new_none();
}

PyObjectPtr BuiltinFunctions::__id__(const ParsedFunctionArguments& args)
{
    auto obj = args.get_arg_named("obj");
    return helpers::new_int(reinterpret_cast<std::uintptr_t>(obj.get()));
}

PyObjectPtr BuiltinFunctions::get_function_named(const std::string& name) const
{
    return functions.at(name);
}