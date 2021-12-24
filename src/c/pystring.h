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

        static PyObjectPtr __bool__(const ParsedFunctionArguments&);
        static PyObjectPtr __repr__(const ParsedFunctionArguments&);
        static PyObjectPtr __str__(const ParsedFunctionArguments&);
        static PyObjectPtr __call__(const ParsedFunctionArguments&);

        static PyObjectPtr capitalize(const ParsedFunctionArguments&);
        static PyObjectPtr casefold(const ParsedFunctionArguments&);
        static PyObjectPtr center(const ParsedFunctionArguments&);
        static PyObjectPtr count(const ParsedFunctionArguments&);
        static PyObjectPtr encode(const ParsedFunctionArguments&);
        static PyObjectPtr endswith(const ParsedFunctionArguments&);
        static PyObjectPtr expandtabs(const ParsedFunctionArguments&);
        static PyObjectPtr find(const ParsedFunctionArguments&);
        static PyObjectPtr format(const ParsedFunctionArguments&);
        static PyObjectPtr format_map(const ParsedFunctionArguments&);
        static PyObjectPtr index(const ParsedFunctionArguments&);
        static PyObjectPtr isalnum(const ParsedFunctionArguments&);
        static PyObjectPtr isalpha(const ParsedFunctionArguments&);
        static PyObjectPtr isascii(const ParsedFunctionArguments&);
        static PyObjectPtr isdecimal(const ParsedFunctionArguments&);
        static PyObjectPtr isdigit(const ParsedFunctionArguments&);
        static PyObjectPtr isidentifier(const ParsedFunctionArguments&);
        static PyObjectPtr islower(const ParsedFunctionArguments&);
        static PyObjectPtr isnumeric(const ParsedFunctionArguments&);
        static PyObjectPtr isprintable(const ParsedFunctionArguments&);
        static PyObjectPtr isspace(const ParsedFunctionArguments&);
        static PyObjectPtr istitle(const ParsedFunctionArguments&);
        static PyObjectPtr isupper(const ParsedFunctionArguments&);
        static PyObjectPtr join(const ParsedFunctionArguments&);
        static PyObjectPtr ljust(const ParsedFunctionArguments&);
        static PyObjectPtr lower(const ParsedFunctionArguments&);
        static PyObjectPtr lstrip(const ParsedFunctionArguments&);
        static PyObjectPtr maketrans(const ParsedFunctionArguments&);
        static PyObjectPtr partition(const ParsedFunctionArguments&);
        static PyObjectPtr replace(const ParsedFunctionArguments&);
        static PyObjectPtr rfind(const ParsedFunctionArguments&);
        static PyObjectPtr rindex(const ParsedFunctionArguments&);
        static PyObjectPtr rjust(const ParsedFunctionArguments&);
        static PyObjectPtr rpartition(const ParsedFunctionArguments&);
        static PyObjectPtr rsplit(const ParsedFunctionArguments&);
        static PyObjectPtr rstrip(const ParsedFunctionArguments&);
        static PyObjectPtr split(const ParsedFunctionArguments&);
        static PyObjectPtr splitlines(const ParsedFunctionArguments&);
        static PyObjectPtr startswith(const ParsedFunctionArguments&);
        static PyObjectPtr strip(const ParsedFunctionArguments&);
        static PyObjectPtr swapcase(const ParsedFunctionArguments&);
        static PyObjectPtr title(const ParsedFunctionArguments&);
        static PyObjectPtr translate(const ParsedFunctionArguments&);
        static PyObjectPtr upper(const ParsedFunctionArguments&);
        static PyObjectPtr zfill(const ParsedFunctionArguments&);

        std::string internal;
    };
}

#endif