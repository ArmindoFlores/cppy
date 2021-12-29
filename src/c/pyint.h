#ifndef _PYINT_H_
#define _PYINT_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyInt : public PyObject {
    public:
        PyInt();
        PyInt(long long x);
        ~PyInt() = default;

        static PyObjectPtr __abs__(const ParsedFunctionArguments&);
        static PyObjectPtr __add__(const ParsedFunctionArguments&);
        static PyObjectPtr __and__(const ParsedFunctionArguments&);
        static PyObjectPtr __bool__(const ParsedFunctionArguments&);
        static PyObjectPtr __ceil__(const ParsedFunctionArguments&);
        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __delattr__(const ParsedFunctionArguments&);
        static PyObjectPtr __dir__(const ParsedFunctionArguments&);
        static PyObjectPtr __divmod__(const ParsedFunctionArguments&);
        static PyObjectPtr __doc__(const ParsedFunctionArguments&);
        static PyObjectPtr __eq__(const ParsedFunctionArguments&);
        static PyObjectPtr __float__(const ParsedFunctionArguments&);
        static PyObjectPtr __floor__(const ParsedFunctionArguments&);
        static PyObjectPtr __floordiv__(const ParsedFunctionArguments&);
        static PyObjectPtr __format__(const ParsedFunctionArguments&);
        static PyObjectPtr __ge__(const ParsedFunctionArguments&);
        static PyObjectPtr __getattribute__(const ParsedFunctionArguments&);
        static PyObjectPtr __getnewargs__(const ParsedFunctionArguments&);
        static PyObjectPtr __gt__(const ParsedFunctionArguments&);
        static PyObjectPtr __hash__(const ParsedFunctionArguments&);
        static PyObjectPtr __index__(const ParsedFunctionArguments&);
        static PyObjectPtr __init__(const ParsedFunctionArguments&);
        static PyObjectPtr __init_subclass__(const ParsedFunctionArguments&);
        static PyObjectPtr __int__(const ParsedFunctionArguments&);
        static PyObjectPtr __invert__(const ParsedFunctionArguments&);
        static PyObjectPtr __le__(const ParsedFunctionArguments&);
        static PyObjectPtr __lshift__(const ParsedFunctionArguments&);
        static PyObjectPtr __lt__(const ParsedFunctionArguments&);
        static PyObjectPtr __mod__(const ParsedFunctionArguments&);
        static PyObjectPtr __mul__(const ParsedFunctionArguments&);
        static PyObjectPtr __ne__(const ParsedFunctionArguments&);
        static PyObjectPtr __neg__(const ParsedFunctionArguments&);
        static PyObjectPtr __new__(const ParsedFunctionArguments&);
        static PyObjectPtr __or__(const ParsedFunctionArguments&);
        static PyObjectPtr __pos__(const ParsedFunctionArguments&);
        static PyObjectPtr __pow__(const ParsedFunctionArguments&);
        static PyObjectPtr __radd__(const ParsedFunctionArguments&);
        static PyObjectPtr __rand__(const PyObject&);
        static PyObjectPtr __rdivmod__(const ParsedFunctionArguments&);
        static PyObjectPtr __reduce__(const ParsedFunctionArguments&);
        static PyObjectPtr __reduce_ex__(const ParsedFunctionArguments&);
        static PyObjectPtr __repr__(const ParsedFunctionArguments&);
        static PyObjectPtr __rfloordiv__(const ParsedFunctionArguments&);
        static PyObjectPtr __rlshift__(const ParsedFunctionArguments&);
        static PyObjectPtr __rmod__(const ParsedFunctionArguments&);
        static PyObjectPtr __rmul__(const ParsedFunctionArguments&);
        static PyObjectPtr __ror__(const ParsedFunctionArguments&);
        static PyObjectPtr __round__(const ParsedFunctionArguments&);
        static PyObjectPtr __rpow__(const ParsedFunctionArguments&);
        static PyObjectPtr __rrshift__(const ParsedFunctionArguments&);
        static PyObjectPtr __rshift__(const ParsedFunctionArguments&);
        static PyObjectPtr __rsub__(const ParsedFunctionArguments&);
        static PyObjectPtr __rtruediv__(const ParsedFunctionArguments&);
        static PyObjectPtr __rxor__(const ParsedFunctionArguments&);
        static PyObjectPtr __setattr__(const ParsedFunctionArguments&);
        static PyObjectPtr __sizeof__(const ParsedFunctionArguments&);
        static PyObjectPtr __str__(const ParsedFunctionArguments&);
        static PyObjectPtr __sub__(const ParsedFunctionArguments&);
        static PyObjectPtr __subclasshook__(const ParsedFunctionArguments&);
        static PyObjectPtr __truediv__(const ParsedFunctionArguments&);
        static PyObjectPtr __trunc__(const ParsedFunctionArguments&);
        static PyObjectPtr __xor__(const ParsedFunctionArguments&);

        static PyObjectPtr as_integer_ratio(const ParsedFunctionArguments&);
        static PyObjectPtr bit_length(const ParsedFunctionArguments&);
        static PyObjectPtr conjugate(const ParsedFunctionArguments&);
        static PyObjectPtr denominator(const PyObject&);
        static PyObjectPtr from_bytes(const ParsedFunctionArguments&);
        static PyObjectPtr imag(const PyObject&);
        static PyObjectPtr numerator(const PyObject&);
        static PyObjectPtr real(const PyObject&);
        static PyObjectPtr to_bytes(const ParsedFunctionArguments&);

        // FIXME: Python uses BIGINTs
        long long value;
    };
}

#endif