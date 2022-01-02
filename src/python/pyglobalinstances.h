#ifndef _PYGLOBALINSTANCES_H_
#define _PYGLOBALINSTANCES_H_

#include "pyobject.h"
#include "pynone.h"
#include <map>
#include <string>

#define GI cppy::global::GlobalInstances::the()

namespace cppy {
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