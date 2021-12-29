#include "pygarbagecollector.h"
using namespace cppy;
using namespace cppy::global;
#include <map>

std::shared_ptr<GarbageCollector> GarbageCollector::instance = nullptr;

GarbageCollector::GarbageCollector() : enabled(true)
{
}

GarbageCollector &GarbageCollector::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<GarbageCollector>(new GarbageCollector());
    }
    return *instance;
}

void GarbageCollector::add_container(PyObjectWPtr wptr)
{
    containers.insert(wptr.lock());
}

void GarbageCollector::add_container(PyObjectPtr ptr)
{
    containers.insert(ptr);
}

std::size_t GarbageCollector::collect()
{
    //! This code is based on CPython's implementation, but is really Q&D
    if (!enabled)
        return 0;
    std::map<PyObjectPtr, std::pair<std::size_t, bool>> to_scan;
    std::set<PyObjectPtr> dropped;

    // Add objects and reference counts to map and remove freed
    // objects alltogether
    for (auto& elem : containers) {
        std::size_t ref_count = elem.use_count();
        if (ref_count != 0)
            to_scan[elem] = std::make_pair(ref_count, false);
        else
            dropped.insert(elem);
    }

    // Find tentatively unreachable objects
    for (auto& elem : to_scan) {
        auto ptr = elem.first;
        for (auto referenced : ptr->getrefs()) {
            auto sptr = referenced.lock();
            if (to_scan.count(sptr)) {
                to_scan.at(sptr).first--;
            }
        }
    }

    // Find which objects are actually unreachable
    for (auto& elem : to_scan) {
        auto ptr = elem.first;

        // If element is reachable and hasn't been visited
        if (elem.second.first > 0 && !elem.second.second) {
            for (auto referenced : ptr->getrefs()) {
                auto sptr = referenced.lock();
                if (to_scan.count(sptr)) {
                    to_scan.at(sptr).first = 1;
                }
            }
        }
        elem.second.second = true;
    }

    // Delete unreachable objects
    std::size_t deleted = 0;
    for (auto &elem : to_scan) {
        if (elem.second.first == 0) {
            containers.erase(elem.first);
            deleted++;
        }
    }
    to_scan.clear();

    for (auto d : dropped)
        containers.erase(d);

    return deleted;
}

std::size_t GarbageCollector::get_count() const
{
    return containers.size();
}

bool GarbageCollector::is_tracked(PyObjectPtr ptr) const
{
    return containers.count(ptr);
}

bool GarbageCollector::isenabled() const
{
    return enabled;
}

void GarbageCollector::enable()
{
    enabled = true;
}

void GarbageCollector::disable()
{
    enabled = false;
}