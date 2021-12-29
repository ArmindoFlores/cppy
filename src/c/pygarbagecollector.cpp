#include "pygarbagecollector.h"
using namespace cppy;
using namespace cppy::global;
#include <map>
#include <iostream>

std::shared_ptr<GarbageCollector> GarbageCollector::instance = nullptr;

GarbageCollector::GarbageCollector() 
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
    std::map<PyObjectPtr, std::pair<std::size_t, bool>> to_scan;
    std::set<PyObjectPtr> dropped;

    // Add objects and reference counts to map and remove freed
    // objects alltogether
    std::cout << "Beginning garbage collection...\nTracked containers: ";
    for (auto& elem : containers) {
        std::size_t ref_count = elem.use_count()-1;
        std::cout << "<" << elem << ", " << ref_count << ">" << ", ";
        if (ref_count != 0)
            to_scan[elem] = std::make_pair(ref_count, false);
        else
            dropped.insert(elem);
    }
    std::cout << std::endl;

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
            std::cout << "Collected " << elem.first << std::endl;
            containers.erase(elem.first);
            deleted++;
        }
    }
    to_scan.clear();

    for (auto d : dropped)
        containers.erase(d);

    return deleted;
}
