#ifndef _PYSTRING_H_
#define _PYSTRING_H_

#include "pyobject.h"
#include "pyfunction.h"

namespace cpy {
    class PyString : public PyObject {
    public:
        PyString() = default;
        PyString(const std::string&);
        ~PyString() = default;

        static PyObjectPtr __bool__(const FunctionArguments&);
        static PyObjectPtr __repr__(const FunctionArguments&);
        static PyObjectPtr __str__(const FunctionArguments&);
        static PyObjectPtr __call__(const FunctionArguments&);

        static PyObjectPtr capitalize(const FunctionArguments&);
        static PyObjectPtr casefold(const FunctionArguments&);
        static PyObjectPtr center(const FunctionArguments&);
        static PyObjectPtr count(const FunctionArguments&);
        static PyObjectPtr encode(const FunctionArguments&);
        static PyObjectPtr endswith(const FunctionArguments&);
        static PyObjectPtr expandtabs(const FunctionArguments&);
        static PyObjectPtr find(const FunctionArguments&);
        static PyObjectPtr format(const FunctionArguments&);
        static PyObjectPtr format_map(const FunctionArguments&);
        static PyObjectPtr index(const FunctionArguments&);
        static PyObjectPtr isalnum(const FunctionArguments&);
        static PyObjectPtr isalpha(const FunctionArguments&);
        static PyObjectPtr isascii(const FunctionArguments&);
        static PyObjectPtr isdecimal(const FunctionArguments&);
        static PyObjectPtr isdigit(const FunctionArguments&);
        static PyObjectPtr isidentifier(const FunctionArguments&);
        static PyObjectPtr islower(const FunctionArguments&);
        static PyObjectPtr isnumeric(const FunctionArguments&);
        static PyObjectPtr isprintable(const FunctionArguments&);
        static PyObjectPtr isspace(const FunctionArguments&);
        static PyObjectPtr istitle(const FunctionArguments&);
        static PyObjectPtr isupper(const FunctionArguments&);
        static PyObjectPtr join(const FunctionArguments&);
        static PyObjectPtr ljust(const FunctionArguments&);
        static PyObjectPtr lower(const FunctionArguments&);
        static PyObjectPtr lstrip(const FunctionArguments&);
        static PyObjectPtr maketrans(const FunctionArguments&);
        static PyObjectPtr partition(const FunctionArguments&);
        static PyObjectPtr replace(const FunctionArguments&);
        static PyObjectPtr rfind(const FunctionArguments&);
        static PyObjectPtr rindex(const FunctionArguments&);
        static PyObjectPtr rjust(const FunctionArguments&);
        static PyObjectPtr rpartition(const FunctionArguments&);
        static PyObjectPtr rsplit(const FunctionArguments&);
        static PyObjectPtr rstrip(const FunctionArguments&);
        static PyObjectPtr split(const FunctionArguments&);
        static PyObjectPtr splitlines(const FunctionArguments&);
        static PyObjectPtr startswith(const FunctionArguments&);
        static PyObjectPtr strip(const FunctionArguments&);
        static PyObjectPtr swapcase(const FunctionArguments&);
        static PyObjectPtr title(const FunctionArguments&);
        static PyObjectPtr translate(const FunctionArguments&);
        static PyObjectPtr upper(const FunctionArguments&);
        static PyObjectPtr zfill(const FunctionArguments&);

        std::string internal;
    };
}

#endif