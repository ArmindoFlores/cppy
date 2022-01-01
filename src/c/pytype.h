#ifndef _PYTYPE_H_
#define _PYTYPE_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyType : public PyObject {
    public:
        PyType(const std::string& name, PyObjectPtr constructor);

        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __mro__(const PyObject&);
        static PyObjectPtr __name__(const PyObject&);
        static PyObjectPtr __call__, __init__, __repr__;
        static PyObjectPtr mro;

        std::string name;
        PyObjectPtr constructor;
    };
}

#endif