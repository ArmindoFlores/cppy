#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <memory>
#include "pyobject.h"
#include "pyfunction.h"

namespace cpy {
    class helpers {
    public:
        static PyObjectPtr new_string(const std::string& s);
        static PyObjectPtr call(PyObjectPtr obj, FunctionArguments args);
        static PyObjectPtr call_member(const std::string&, PyObjectPtr obj, FunctionArguments args);
    };
}

#endif