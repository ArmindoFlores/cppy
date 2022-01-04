#ifndef _PYTYPE_H_
#define _PYTYPE_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyType : public PyObject {
    public:
        PyType(const std::string& name, std::function<void(PyObject*)>, PyObjectPtr bases);

        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __mro__(const PyObject&);
        static PyObjectPtr __name__(const PyObject&);
        static PyObjectPtr __call__, __init__, __repr__, __new__;
        static PyObjectPtr mro;

        static void construct(PyObject*);

        std::string name;
    };
}

#endif