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

        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __rand__(const PyObject&);
        static PyObjectPtr __abs__, __add__, __and__, __bool__, __ceil__, __delattr__, __dir__, __divmod__, __doc__, __eq__, __float__, __floor__, __floordiv__, __format__, __ge__, __getattribute__, __getnewargs__, __gt__, __hash__, __index__, __init__, __init_subclass__, __int__, __invert__, __le__, __lshift__, __lt__, __mod__, __mul__, __ne__, __neg__, __new__, __or__, __pos__, __pow__, __radd__, __rdivmod__, __reduce__, __reduce_ex__, __repr__, __rfloordiv__, __rlshift__, __rmod__, __rmul__, __ror__, __round__, __rpow__, __rrshift__, __rshift__, __rsub__, __rtruediv__, __rxor__, __setattr__, __sizeof__, __sub__, __subclasshook__, __truediv__, __trunc__, __xor__, as_integer_ratio, bit_length, conjugate, from_bytes, to_bytes;
        
        static PyObjectPtr denominator(const PyObject&);
        static PyObjectPtr imag(const PyObject&);
        static PyObjectPtr numerator(const PyObject&);
        static PyObjectPtr real(const PyObject&);

        static void construct(PyObject*);

        // FIXME: Python uses BIGINTs
        long long value;
    };
}

#endif