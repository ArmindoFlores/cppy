#ifndef _PYBUILTINS_H_
#define _PYBUILTINS_H_

#include <memory>
#include "pyobject.h"
#include "pyfunction.h"

namespace cpy {
    namespace globals {
        class BuiltinFunctions {
        public:
            static BuiltinFunctions &the();

            PyObjectPtr get_function_named(const std::string& name) const;

        private:
            static std::shared_ptr<BuiltinFunctions> instance;
            std::map<std::string, PyObjectPtr> functions;
            
            BuiltinFunctions();

            static PyObjectPtr __print__(const ParsedFunctionArguments& obj);
            static PyObjectPtr __repr__(const ParsedFunctionArguments& obj);
        };
    }
}

#endif