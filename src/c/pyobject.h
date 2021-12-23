#ifndef _PYOBJECT_H_
#define _PYOBJECT_H_

#include <map>
#include <string>
#include <memory>

namespace cpy {

    class PyObject {
    public:
        PyObject() = default;
        virtual ~PyObject() = default;

        std::shared_ptr<PyObject> getattr(const std::string& name) const;
        void setattr(const std::string& name, std::shared_ptr<PyObject>);
        bool hasattr(const std::string& name) const;
        
        template<typename T>
        T& as()
        {
            return *dynamic_cast<T*>(this);
        }

    protected:
        std::map<std::string, std::shared_ptr<PyObject>> attributes;
    };

    typedef std::shared_ptr<PyObject> PyObjectPtr;
}

#endif