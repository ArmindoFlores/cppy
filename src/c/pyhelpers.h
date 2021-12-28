#ifndef _PYHELPERS_H_
#define _PYHELPERS_H_

#include <memory>
#include "pyobject.h"
#include "pyfunction.h"

namespace cppy {
    class helpers {
    public:
        static PyObjectPtr new_string(const std::string& s);
        static PyObjectPtr new_int(long long x);
        static PyObjectPtr new_none();
        static PyObjectPtr call(PyObjectPtr callable, FunctionArguments args);
        static PyObjectPtr call_member(const std::string&, PyObjectPtr obj, FunctionArguments args);
    };
}

#endif