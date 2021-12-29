#ifndef _PYNOTIMPLEMENTED_H_
#define _PYNOTIMPLEMENTED_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyNotImplemented : public PyObject {
    public:
        PyNotImplemented();

        ~PyNotImplemented() = default;

        static PyObjectPtr __bool__(const ParsedFunctionArguments&);
        static PyObjectPtr __repr__(const ParsedFunctionArguments&);
        static PyObjectPtr __str__(const ParsedFunctionArguments&);
        static PyObjectPtr __class__(const PyObject&);
    };
}

#endif