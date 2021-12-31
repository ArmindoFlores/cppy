#ifndef _PYTYPE_H_
#define _PYTYPE_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyType : public PyObject {
    public:
        PyType(const std::string& name, PyObjectPtr constructor);

        static PyObjectPtr __name__(const PyObject&);
        static PyObjectPtr __call__, __repr__;

        std::string name;
        PyObjectPtr constructor;
    };
}

#endif