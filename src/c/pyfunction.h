#ifndef _PYWRAPPERFUNCTION_H_
#define _PYWRAPPERFUNCTION_H_

#include <functional>
#include <vector>
#include "pyobject.h"

namespace cpy {
    class FunctionArgument {
    public:
        FunctionArgument() = default;
        FunctionArgument(PyObjectPtr, const std::string& name = "");
        bool is_named() const;
        PyObjectPtr get_object() const;
        std::string get_name() const; 

    private:
        PyObjectPtr o;
        std::string name;
    };

    typedef std::vector<FunctionArgument> FunctionArguments;

    class PyFunction : public PyObject {
    public:
        PyFunction(std::function<PyObjectPtr(const FunctionArguments&)>, const std::string&);
        ~PyFunction() = default;

        PyObjectPtr call(const FunctionArguments&);

    private:
        std::function<PyObjectPtr(const FunctionArguments&)> internal;
        std::string function_name;
    };
}

#endif