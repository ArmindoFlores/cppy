#include "pyrecursionctx.h"
using namespace cppy;
using namespace cppy::global;

std::shared_ptr<RecursionContext> RecursionContext::instance = nullptr;

RecursionContext::RecursionContext() {}

RecursionContext &RecursionContext::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<RecursionContext>(new RecursionContext());
    }
    return *instance;
}

bool RecursionContext::enter(PyObject* ptr)
{
    if (ptrs.count(ptr))
        return false;
    ptrs.insert(ptr);
    return true;
}

void RecursionContext::exit(PyObject* ptr)
{
    ptrs.erase(ptr);
}