#ifndef _PYOBJECT_H_
#define _PYOBJECT_H_

#include <map>
#include <string>
#include <memory>
#include <variant>
#include <functional>

namespace cppy {

    class PyObject {
    public:
        PyObject();
        virtual ~PyObject() = default;

        virtual std::shared_ptr<PyObject> getattr(const std::string& name) const;
        virtual void setattr(const std::string& name, std::variant<std::shared_ptr<PyObject>, std::function<std::shared_ptr<PyObject>(const PyObject&)>>);
        virtual bool hasattr(const std::string& name) const;
        
        template<typename T>
        T* as()
        {
            return dynamic_cast<T*>(this);
        }

        virtual bool gccollected();
        virtual std::vector<std::weak_ptr<PyObject>> getrefs();

    protected:
        std::unordered_map<std::string, std::variant<std::shared_ptr<PyObject>, std::function<std::shared_ptr<PyObject>(const PyObject&)>>> attributes;
    };

    typedef std::shared_ptr<PyObject> PyObjectPtr;
    typedef std::weak_ptr<PyObject> PyObjectWPtr;
    typedef std::variant<PyObjectPtr, std::function<PyObjectPtr(const PyObject&)>> PyObjectPtrOrFunc;
    typedef std::variant<PyObjectPtr, PyObjectWPtr> PyObjectAnyPtr;
}

#endif