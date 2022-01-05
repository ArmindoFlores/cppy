#ifndef _PYBOOL_H_
#define _PYBOOL_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyBool : public PyObject {
    public:
        PyBool();
        PyBool(bool);

        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __repr__, __int__;
        static void construct(PyObject*);

        bool value;
    };
}

#endif