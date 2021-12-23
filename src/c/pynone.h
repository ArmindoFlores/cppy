#ifndef _PYNONE_H_
#define _PYNONE_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cpy {
    class PyNone : public PyObject {
    public:
        PyNone();
        ~PyNone() = default;

        static PyObjectPtr __bool__(const FunctionArguments&);
        static PyObjectPtr __repr__(const FunctionArguments&);
        static PyObjectPtr __str__(const FunctionArguments&);
        static PyObjectPtr __call__(const FunctionArguments&);

    };
}

#endif