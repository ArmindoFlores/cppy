#include "pyglobalinstances.h"
#include "pynone.h"
#include "pynotimplemented.h"
#include "pybool.h"
using namespace cppy;
using namespace cppy::global;

std::shared_ptr<GlobalInstances> GlobalInstances::instance = nullptr;

GlobalInstances::GlobalInstances() 
{
    instances["none"] = std::make_shared<PyNone>();
    instances["notimpl"] = std::make_shared<PyNotImplemented>();
    instances["True"] = std::make_shared<PyBool>(true);
    instances["False"] = std::make_shared<PyBool>(false);
}

GlobalInstances &GlobalInstances::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<GlobalInstances>(new GlobalInstances());
    }
    return *instance;
}

PyObjectPtr GlobalInstances::get(const std::string& name) const
{
    return instances.at(name);
}