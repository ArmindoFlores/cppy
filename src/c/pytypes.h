#ifndef _PYTYPES_H_
#define _PYTYPES_H_

#include "pyobject.h"
#include "pytype.h"
#include <memory>
#include <map>
#include <string>

#define BT cpy::globals::BuiltinTypes::the()

namespace cpy {
    namespace globals {
        class BuiltinTypes {
        public:
            static BuiltinTypes &the();

            PyObjectPtr get_type_named(const std::string& name) const;

        private:
            static std::shared_ptr<BuiltinTypes> instance;
            std::map<std::string, PyObjectPtr> types;
            
            BuiltinTypes();

            static PyObjectPtr __str__(const ParsedFunctionArguments&);
            static PyObjectPtr __int__(const ParsedFunctionArguments&);
            static PyObjectPtr __none__(const ParsedFunctionArguments&);
        };
    }
}

#endif