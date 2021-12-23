#include "pytraceback.h"
using namespace cpy::globals;
#include <iostream>

traceback_exception::traceback_exception(const std::string& signature) : signature(signature) {}

const char *traceback_exception::what() const noexcept
{
    return signature.c_str();
}

Traceback *Traceback::instance = nullptr;

Traceback::Traceback() {}

Traceback &Traceback::the()
{
    if (instance == nullptr) {
        instance = new Traceback();
    }
    return *instance;
}

void Traceback::push(const std::string& s)
{
    tb.push_front(s);
}

void Traceback::pop()
{
    tb.pop_front();
}

void Traceback::raise(const std::string& msg, const std::string& signature)
{
    if (tb.empty())
        throw std::bad_exception();

    std::string message = create_message(msg);

    while (!tb.empty() && tb.front() != signature) {
        tb.pop_front();
    }

    throw traceback_exception(message);
}

std::string Traceback::create_message(const std::string& msg) const
{
    std::string result = "Traceback (most recent call last):\n";
    for (const std::string& element : tb) {
        result += "  File X, line X in " + element;
        result += "\n    X(...)\n";
    }
    result += msg + "\n";
    return result;
}