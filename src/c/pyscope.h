#ifndef _PYSCOPE_H_
#define _PYSCOPE_H_

#include "pyobject.h"
#include <memory>

#define SCOPE cppy::global::Scope::the()

namespace cppy {
    namespace global {
        class Scope {
        public:
            static Scope &the();

            void add_scope(const std::string& name);
            std::map<std::string, PyObjectPtr>& get_scope(const std::string& name);
            PyObjectPtr get_var(const std::string& var, const std::string& scope) const;
            void set_var(const std::string& var, const std::string& scope, PyObjectPtr value);

        private:
            static std::shared_ptr<Scope> instance;
            Scope();

        protected:
            std::unordered_map<std::string, std::map<std::string, PyObjectPtr>> scopes;
        };
    }
}

#endif