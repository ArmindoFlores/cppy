#ifndef _PYBUILTINS_H_
#define _PYBUILTINS_H_

#include <memory>
#include "pyobject.h"
#include "pyfunction.h"

#define BF cppy::globals::BuiltinFunctions::the()

namespace cppy {
    namespace globals {
        class BuiltinFunctions {
        public:
            static BuiltinFunctions &the();

            PyObjectPtr get_function_named(const std::string& name) const;

        private:
            static std::shared_ptr<BuiltinFunctions> instance;
            std::unordered_map<std::string, PyObjectPtr> functions;
            
            BuiltinFunctions();

            static PyObjectPtr __print__(const ParsedFunctionArguments& obj);
            static PyObjectPtr __input__(const ParsedFunctionArguments& obj);
            static PyObjectPtr __repr__(const ParsedFunctionArguments& obj);
            static PyObjectPtr __id__(const ParsedFunctionArguments& obj);
            static PyObjectPtr __dir__(const ParsedFunctionArguments& obj);
            static PyObjectPtr __type__(const ParsedFunctionArguments& obj);
        };
    }
}

#endif