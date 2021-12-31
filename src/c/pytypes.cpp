#include "pytypes.h"
#include "pyhelpers.h"
#include "pybuiltins.h"
#include "pyint.h"
#include "pylist.h"
#include "pygarbagecollector.h"
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

    // <class 'NotImplemented'>
    types["notimpl"] = std::make_shared<PyType>(
        "NotImplemented",
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

    // <class 'list'>
    types["list"] = std::make_shared<PyType>(
        "list",
        std::make_shared<PyFunction>(
            __list__,
            "list"
        )
    );

    // <class 'dict'>
    types["dict"] = std::make_shared<PyType>(
        "dict",
        std::make_shared<PyFunction>(
            __dict__,
            "dict"
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
    auto base = args.get_arg_named("base")->as<PyInt>()->value;
    auto value = args.get_arg_named("x")->as<PyInt>()->value;
    if (base != 10) {
        TB.raise("base != 10 is not implemented", "NotImplementedError");
    }
    return helpers::new_int(value);
}

PyObjectPtr BuiltinTypes::__none__(const ParsedFunctionArguments& args)
{
    return GI.get("none");
}

PyObjectPtr BuiltinTypes::__notimpl__(const ParsedFunctionArguments& args)
{
    return GI.get("notimpl");
}

PyObjectPtr BuiltinTypes::__list__(const ParsedFunctionArguments& args)
{
    return helpers::new_list();
}

PyObjectPtr BuiltinTypes::__dict__(const ParsedFunctionArguments& args)
{
    return helpers::new_dict();
}