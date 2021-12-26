#include "pyglobalinstances.h"
#include "pynone.h"
using namespace cpy;
using namespace cpy::global;

std::shared_ptr<GlobalInstances> GlobalInstances::instance = nullptr;

GlobalInstances::GlobalInstances() 
{
    instances["none"] = std::make_shared<PyNone>();
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