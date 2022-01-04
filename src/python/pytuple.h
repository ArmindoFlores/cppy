#ifndef _PYTUPLE_H_
#define _PYTUPLE_H_

#include "pyobject.h"
#include "pyfunction.h"
#include <vector>

namespace cppy {
    class PyTuple : public PyGCObject {
    public:
        PyTuple(const std::vector<PyObjectAnyPtr>&);

        virtual bool gccollected() const override;
        std::vector<PyObjectWPtr> getrefs() override;

        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr __hash__(const PyObject&);
        static PyObjectPtr __bool__, __len__, __repr__, __str__;

        static void construct(PyObject*);

        std::vector<PyObjectAnyPtr> internal;
    };
}

#endif