#include "pytypes.h"
#include "pyhelpers.h"
#include "pybuiltins.h"
#include "pyint.h"
#include "pyglobalinstances.h"
#include "pytraceback.h"
using namespace cppy;
using namespace cppy::globals;

std::shared_ptr<BuiltinTypes> BuiltinTypes::instance = nullptr;

BuiltinTypes::BuiltinTypes()
{
    // <class 'NoneType'>
    types["none"] = std::make_shared<PyType>(
        "NoneType",
        std::make_shared<PyFunction>(
            __none__,
            "none"
        )
    );

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

    // <class 'int'>
    types["int"] = std::make_shared<PyType>(
        "int",
        std::make_shared<PyFunction>(
            __int__,
            "int",
            std::vector<std::string>({"x", "base"}),
            std::vector<PyObjectPtr>({helpers::new_int(10)}),
            1
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

PyObjectPtr BuiltinTypes::__int__(const ParsedFunctionArguments& args)
{
    // FIXME: currently we can only make integers out of other integers
    auto base = args.get_arg_named("base")->as<PyInt>().value;
    auto value = args.get_arg_named("x")->as<PyInt>().value;
    if (base != 10) {
        TB.raise("NotImplementedError: base != 10 is not implemented", "NotImplementedError");
    }
    return std::make_shared<PyInt>(value);
}

PyObjectPtr BuiltinTypes::__none__(const ParsedFunctionArguments& args)
{
    return GI.get("none");
}