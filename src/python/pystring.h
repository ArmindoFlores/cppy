#ifndef _PYSTRING_H_
#define _PYSTRING_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class PyString : public PyObject {
    public:
        PyString();
        PyString(const std::string&);
        ~PyString() = default;

        static PyObjectPtr __bool__, __repr__, __str__;
        static PyObjectPtr __class__(const PyObject&);
        static PyObjectPtr capitalize, casefold, center, count, encode, endswith, expandtabs, find, format, format_map, index, isalnum, isalpha, isascii, isdecimal, isdigit, isidentifier, islower, isnumeric, isprintable, isspace, istitle, isupper, join, ljust, lower, lstrip, maketrans, partition, replace, rfind, rindex, rjust, rpartition, rsplit, rstrip, split, splitlines, startswith, strip, swapcase, title, translate, upper, zfill;
        
        static void construct(PyObject*);

        std::string internal;
    };
}

#endif