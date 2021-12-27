#ifndef _PYINT_H_
#define _PYINT_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cpy {
    class PyInt : public PyObject {
    public:
        PyInt();
        PyInt(long long x);
        ~PyInt() = default;

        static PyObjectPtr __bool__(const ParsedFunctionArguments&);
        static PyObjectPtr __repr__(const ParsedFunctionArguments&);
        static PyObjectPtr __str__(const ParsedFunctionArguments&);
        static PyObjectPtr __class__(const PyObject&);

        // FIXME: Python uses BIGINTs
        long long value;
    };
}

#endif