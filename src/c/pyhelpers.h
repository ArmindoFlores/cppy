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
        static PyObjectPtr new_dict();
        static PyObjectPtr new_list();
        static PyObjectPtr new_list(const std::vector<PyObjectPtr>&);
        static PyObjectPtr new_none();
        static PyObjectPtr new_notimpl();
        static PyObjectPtr call(PyObjectPtr callable, FunctionArguments args);
        static PyObjectPtr call_member(const std::string&, PyObjectPtr obj, FunctionArguments args);
        static bool is(PyObjectPtr, PyObjectPtr);
        
        static PyObjectPtr add(PyObjectPtr, PyObjectPtr);
        static PyObjectPtr sub(PyObjectPtr, PyObjectPtr);
        static PyObjectPtr mul(PyObjectPtr, PyObjectPtr);
        static PyObjectPtr divmod(PyObjectPtr, PyObjectPtr);
        static PyObjectPtr truediv(PyObjectPtr, PyObjectPtr);
        static PyObjectPtr ceil(PyObjectPtr, PyObjectPtr);
        static PyObjectPtr floor(PyObjectPtr, PyObjectPtr);
    };
}

#endif