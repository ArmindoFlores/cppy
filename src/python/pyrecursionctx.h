#ifndef _PYRECURSIONCTX_H_
#define _PYRECURSIONCTX_H_

#include "pyobject.h"
#include <string>
#include <set>

#define RC cppy::global::RecursionContext::the()

namespace cppy {
    namespace global {
        class RecursionContext {
        public:
            static RecursionContext &the();
            bool enter(PyObject*);
            void exit(PyObject*);

        private:
            static std::shared_ptr<RecursionContext> instance;
            RecursionContext();

        protected:
            std::set<PyObject*> ptrs;
        };
    }
}

#endif