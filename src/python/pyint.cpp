#include "pyint.h"
#include "pystring.h"
#include "pytypes.h"
#include "pyhelpers.h"
using namespace cppy;


static PyObjectPtr PyInt__repr__(const ParsedFunctionArguments &args)
{
    return helpers::new_string(std::to_string(args.get_arg_named("self")->as<PyInt>()->value));
}

static PyObjectPtr PyInt__hash__(const ParsedFunctionArguments &args)
{
    return args.get_arg_named("self");
}

static PyObjectPtr PyInt__add__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(self->value + other->as<PyInt>()->value);
    }

    return helpers::new_notimpl();
}

static PyObjectPtr PyInt__radd__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(other->as<PyInt>()->value + self->value);
    }

    return helpers::new_notimpl();
}

static PyObjectPtr PyInt__sub__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(self->value - other->as<PyInt>()->value);
    }

    return helpers::new_notimpl();
}

static PyObjectPtr PyInt__rsub__(const ParsedFunctionArguments &args)
{
    auto self = args.get_arg_named("self")->as<PyInt>();
    auto other = args.get_arg_named("other");

    // FIXME: use isinstance()
    if (other->getattr("__class__") == BT.get_type_named("int")) {
        return helpers::new_int(other->as<PyInt>()->value - self->value);
    }

    return helpers::new_notimpl();
}

PyObjectPtr PyInt::__hash__ = std::make_shared<PyFunction>(PyInt__hash__, "__hash__", std::vector<std::string>({"self"}));
PyObjectPtr PyInt::__add__ = std::make_shared<PyFunction>(PyInt__add__, "__add__", std::vector<std::string>({"self", "other"}));
PyObjectPtr PyInt::__radd__ = std::make_shared<PyFunction>(PyInt__radd__, "__radd__", std::vector<std::string>({"self", "other"}));
PyObjectPtr PyInt::__sub__ = std::make_shared<PyFunction>(PyInt__sub__, "__sub__", std::vector<std::string>({"self", "other"}));
PyObjectPtr PyInt::__rsub__ = std::make_shared<PyFunction>(PyInt__rsub__, "__rsub__", std::vector<std::string>({"self", "other"}));
PyObjectPtr PyInt::__repr__ = std::make_shared<PyFunction>(PyInt__repr__, "__repr__", std::vector<std::string>({"self"}));

PyObjectPtr PyInt::__class__(const PyObject&)
{
    return BT.get_type_named("int");
}

void PyInt::construct(PyObject *self)
{
    self->setattr("__hash__", __hash__);
    self->setattr("__add__", __add__);
    self->setattr("__radd__", __radd__);
    self->setattr("__sub__", __sub__);
    self->setattr("__rsub__", __rsub__);
    self->setattr("__repr__", __repr__);
}

PyInt::PyInt()
{
    setattr("__class__", __class__);
}

PyInt::PyInt(long long x) : PyInt()
{
    value = x;
}

