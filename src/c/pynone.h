#ifndef _PYNONE_H_
#define _PYNONE_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cpy {
    class PyNone : public PyObject {
    public:
        PyNone();
        ~PyNone() = default;

        static PyObjectPtr __bool__(const ParsedFunctionArguments&);
        static PyObjectPtr __repr__(const ParsedFunctionArguments&);
        static PyObjectPtr __str__(const ParsedFunctionArguments&);
        static PyObjectPtr __call__(const ParsedFunctionArguments&);

    };
}

#endif