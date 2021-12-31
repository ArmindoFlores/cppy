#ifndef _PYDICT_H_
#define _PYDICT_H_

#include "pyobject.h"
#include "pyfunction.h"


namespace cppy {
    class CustomCompare {
    public:
        bool operator()(const PyObjectAnyPtr&, const PyObjectAnyPtr&) const;
    };

    class PyDict : public PyObject {
    public:
        PyDict();

        std::vector<PyObjectWPtr> getrefs() override;
        bool gccollected() override;

        static PyObjectPtr __repr__, __len__, __getitem__, __setitem__, __delitem__;
        static PyObjectPtr __class__(const PyObject&);
        
        std::map<PyObjectAnyPtr, PyObjectAnyPtr, CustomCompare> internal;
    };
}

#endif