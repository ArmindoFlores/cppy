#include "pyint.h"
#include "pystring.h"
#include "pytypes.h"
#include "pyhelpers.h"
using namespace cppy;

PyInt::PyInt()
{
    setattr("__add__", std::make_shared<PyFunction>(__add__, "__add__", std::vector<std::string>({"self", "other"})));
    setattr("__radd__", std::make_shared<PyFunction>(__radd__, "__radd__", std::vector<std::string>({"self", "other"})));
    setattr("__sub__", std::make_shared<PyFunction>(__sub__, "__sub__", std::vector<std::string>({"self", "other"})));
    setattr("__rsub__", std::make_shared<PyFunction>(__rsub__, "__rsub__", std::vector<std::string>({"self", "other"})));
    setattr("__str__", std::make_shared<PyFunction>(__str__, "__str__", std::vector<std::string>({"self"})));
    setattr("__repr__", std::make_shared<PyFunction>(__repr__, "__repr__", std::vector<std::string>({"self"})));
    setattr("__class__", __class__);
}

PyInt::PyInt(long long x) : PyInt()
{
    value = x;
}

PyObjectPtr PyInt::__str__(const ParsedFunctionArguments &args)
{
    return __repr__(args);
}

PyObjectPtr PyInt::__repr__(const ParsedFunctionArguments &args)
{
    return std::make_shared<PyString>(std::to_string(args.get_arg_named("self")->as<PyInt>()->value));
}

PyObjectPtr PyInt::__class__(const PyObject&)
{
    return BT.get_type_named("int");
}

PyObjectPtr PyInt::__add__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(self->value + other->as<PyInt>()->value);
    }

    return helpers::new_notimpl();
}

PyObjectPtr PyInt::__radd__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(other->as<PyInt>()->value + self->value);
    }

    return helpers::new_notimpl();
}

PyObjectPtr PyInt::__sub__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(self->value - other->as<PyInt>()->value);
    }

    return helpers::new_notimpl();
}

PyObjectPtr PyInt::__rsub__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(other->as<PyInt>()->value - self->value);
    }

    return helpers::new_notimpl();
}

