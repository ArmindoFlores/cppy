#ifndef _PYBUILTINS_H_
#define _PYBUILTINS_H_

#include <memory>
#include "pyobject.h"

namespace cpy {
    class builtins {
    public:
        static PyObjectPtr __print__(PyObjectPtr obj);
        static PyObjectPtr __repr__(PyObjectPtr obj);
        static PyObjectPtr __str__(PyObjectPtr obj);
        static PyObjectPtr __bool__(PyObjectPtr obj);
    };
}

#endif