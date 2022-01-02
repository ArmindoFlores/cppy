#ifndef _PYLIST_H_
#define _PYLIST_H_

#include "pyobject.h"
#include "pyfunction.h"
#include <vector>

namespace cppy {
    class PyList : public PyGCObject {
    public:
        PyList();

        std::vector<PyObjectWPtr> getrefs() override;

        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __hash__(const PyObject&);
        static PyObjectPtr __bool__, __len__, __repr__, __str__;
        static PyObjectPtr append, pop;

        std::vector<PyObjectAnyPtr> internal;
    };
}

#endif