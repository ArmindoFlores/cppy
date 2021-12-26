#include "pytypes.h"
#include "pyhelpers.h"
#include "pybuiltins.h"
#include "pyglobalinstances.h"
using namespace cpy;
using namespace cpy::globals;

std::shared_ptr<BuiltinTypes> BuiltinTypes::instance = nullptr;

BuiltinTypes::BuiltinTypes()
{
    // <class 'str'>
    types["str"] = std::make_shared<PyType>(
        "str",
        std::make_shared<PyFunction>(
            __str__,
            "str",
            std::vector<std::string>({"object"}),
            std::vector<PyObjectPtr>({helpers::new_string("")}),
            0
        )
    );

    // <class 'NoneType'>
    types["none"] = std::make_shared<PyType>(
        "NoneType",
        std::make_shared<PyFunction>(
            __none__,
            "none"
        )
    );
}

BuiltinTypes &BuiltinTypes::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<BuiltinTypes>(new BuiltinTypes());
    }
    return *instance;
}

PyObjectPtr BuiltinTypes::get_type_named(const std::string& name) const
{
    return types.at(name);
}

PyObjectPtr BuiltinTypes::__str__(const ParsedFunctionArguments& args)
{
    if (args.get_arg_named("object")->hasattr("__str__")) {
        return helpers::call_member("__str__", args.get_arg_named("object"), FunctionArguments());
    }
    return helpers::call_member("__repr__", args.get_arg_named("object"), FunctionArguments());
}

PyObjectPtr BuiltinTypes::__none__(const ParsedFunctionArguments& args)
{
    return GI.get("none");
}