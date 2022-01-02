#ifndef _PYNOTIMPLEMENTED_H_
#define _PYNOTIMPLEMENTED_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyNotImplemented : public PyObject {
    public:
        PyNotImplemented();

        ~PyNotImplemented() = default;

        static PyObjectPtr __bool__, __repr__, __str__;
        static PyObjectPtr __class__(const PyObject&);
    };
}

#endif