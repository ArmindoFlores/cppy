#ifndef _PYTYPES_H_
#define _PYTYPES_H_

#include "pyobject.h"
#include "pytype.h"
#include <memory>
#include <map>
#include <string>

#define BT cppy::globals::BuiltinTypes::the()

namespace cppy {
    namespace globals {
        class BuiltinTypes {
        public:
            static BuiltinTypes &the();

            PyObjectPtr get_type_named(const std::string& name) const;
            bool is_builtin_type(PyObjectPtr) const;

        private:
            static std::shared_ptr<BuiltinTypes> instance;
            std::unordered_map<std::string, PyObjectPtr> types;
            
            BuiltinTypes();

            static PyObjectPtr __type__(const ParsedFunctionArguments&);
            static PyObjectPtr __object__(const ParsedFunctionArguments&);
            static PyObjectPtr __str__(const ParsedFunctionArguments&);
            static PyObjectPtr __int__(const ParsedFunctionArguments&);
            static PyObjectPtr __none__(const ParsedFunctionArguments&);
            static PyObjectPtr __dict__(const ParsedFunctionArguments&);
            static PyObjectPtr __list__(const ParsedFunctionArguments&);
            static PyObjectPtr __notimpl__(const ParsedFunctionArguments&);

            void init_builtin_types();
        };
    }
}

#endif