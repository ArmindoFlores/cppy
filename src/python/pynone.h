#ifndef _PYNONE_H_
#define _PYNONE_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyNone : public PyObject {
    public:
        PyNone();
        ~PyNone() = default;

        static PyObjectPtr __bool__, __repr__, __str__;
        static PyObjectPtr __class__(const PyObject&);

    };
}

#endif