#ifndef _PYBASEOBJECT_H_
#define _PYBASEOBJECT_H_

#include "pyobject.h"

namespace cppy {
    class PyBaseObject : public PyObject {
    public:
        PyBaseObject();

        static PyObjectPtr __repr__, __str__, __hash__, __init__;
        static PyObjectPtr __class__(const PyObject&);
        static void construct(PyObject*);
    };
}

#endif