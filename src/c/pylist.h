#ifndef _PYLIST_H_
#define _PYLIST_H_

#include "pyobject.h"
#include "pyfunction.h"
#include <vector>

namespace cppy {
    class PyList : public PyObject {
    public:
        PyList();

        std::vector<PyObjectWPtr> getrefs() override;
        bool gccollected() override;

        static PyObjectPtr __bool__(const ParsedFunctionArguments&);
        static PyObjectPtr __repr__(const ParsedFunctionArguments&);
        static PyObjectPtr __str__(const ParsedFunctionArguments&);
        static PyObjectPtr __len__(const ParsedFunctionArguments&);
        static PyObjectPtr __class__(const PyObject&);

        static PyObjectPtr append(const ParsedFunctionArguments&);
        static PyObjectPtr pop(const ParsedFunctionArguments&);

        std::vector<std::variant<PyObjectPtr, PyObjectWPtr>> internal;
    };
}

#endif