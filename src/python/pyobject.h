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
        virtual void setattr(const std::string& name, std::shared_ptr<PyObject>);
        virtual void setattr(const std::string& name, std::weak_ptr<PyObject>);
        virtual void setattr(const std::string& name, std::function<std::shared_ptr<PyObject>(const PyObject&)>);
        virtual bool hasattr(const std::string& name) const;
        
        template<typename T>
        T* as()
        {
            return dynamic_cast<T*>(this);
        }

        virtual bool gccollected() const;
        virtual std::vector<std::weak_ptr<PyObject>> getrefs();

    protected:
        std::unordered_map<std::string, std::variant<std::shared_ptr<PyObject>, std::weak_ptr<PyObject>, std::function<std::shared_ptr<PyObject>(const PyObject&)>>> attributes;
        std::shared_ptr<PyObject> from_variant(const std::variant<std::shared_ptr<PyObject>, std::weak_ptr<PyObject>, std::function<std::shared_ptr<PyObject>(const PyObject&)>>&) const;
    };

    typedef std::shared_ptr<PyObject> PyObjectPtr;
    typedef std::weak_ptr<PyObject> PyObjectWPtr;
    typedef std::variant<PyObjectPtr, PyObjectWPtr> PyObjectAnyPtr;

    class PyGCObject : public PyObject {
    public:
        using PyObject::PyObject;
        virtual ~PyGCObject() = default;

        virtual void setattr(const std::string& name, PyObjectPtr) override;
        virtual void setattr(const std::string& name, PyObjectWPtr) override;
        virtual void setattr(const std::string& name, std::function<std::shared_ptr<PyObject>(const PyObject&)>) override;
        virtual bool gccollected() const override;
    };

    class PyUserObject : public PyGCObject {
    public:
        using PyGCObject::PyGCObject;
        virtual ~PyUserObject() = default;

        virtual std::vector<PyObjectWPtr> getrefs() override;
    };
}

#endif