#include "pytraceback.h"
using namespace cppy::globals;
#include <iostream>

traceback_exception::traceback_exception(const std::string& signature) : signature(signature) {}

const char *traceback_exception::what() const noexcept
{
    return signature.c_str();
}

std::shared_ptr<Traceback> Traceback::instance = nullptr;

Traceback::Traceback() {}

Traceback &Traceback::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<Traceback>(new Traceback());
    }
    return *instance;
}

void Traceback::push(const std::string& s)
{
    tb.push_back(s);
}

void Traceback::pop()
{
    tb.pop_back();
}

void Traceback::raise(const std::string& msg, const std::string& signature)
{
    if (tb.empty())
        throw std::bad_exception();

    std::string message = create_message(signature + ": " + msg);

    while (!tb.empty() && tb.back() != signature) {
        tb.pop_back();
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