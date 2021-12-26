#ifndef _PYGLOBALINSTANCES_H_
#define _PYGLOBALINSTANCES_H_

#include "pyobject.h"
#include "pynone.h"
#include <map>
#include <string>

#define GI cpy::global::GlobalInstances::the()

namespace cpy {
    namespace global {
        class GlobalInstances {
        public:
            static GlobalInstances &the();

            PyObjectPtr get(const std::string& name) const;

        private:
            static std::shared_ptr<GlobalInstances> instance;
            GlobalInstances();

        protected:
            std::map<std::string, PyObjectPtr> instances;
        };
    }
}

#endif