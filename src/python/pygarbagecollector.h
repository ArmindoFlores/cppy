#ifndef _PYGARBAGECOLLECTOR_H_
#define _PYGARBAGECOLLECTOR_H_

#include "pyobject.h"
#include <set>
#include <memory>

#define GC cppy::global::GarbageCollector::the()

namespace cppy {
    namespace global {
        class GarbageCollector {
        public:
            static GarbageCollector &the();
            std::size_t collect();
            void add_container(PyObjectWPtr);
            void add_container(PyObjectPtr);
            std::size_t get_count() const;
            bool is_tracked(PyObjectPtr) const;
            bool isenabled() const;
            void enable();
            void disable();

        private:
            GarbageCollector();
            static std::shared_ptr<GarbageCollector> instance;

        protected:
            std::set<PyObjectPtr> containers;
            bool enabled;
        };
    }
}

#endif