#include "pyscope.h"
#include "pytraceback.h"
#include "pyhelpers.h"
using namespace cppy;
using namespace cppy::global;

std::shared_ptr<Scope> Scope::instance = nullptr;

Scope::Scope() {}

Scope &Scope::the()
{
    if (instance == nullptr) {
        instance = std::shared_ptr<Scope>(new Scope());
    }
    return *instance;
}

void Scope::add_scope(const std::string& name)
{
    scopes[name];
}

std::map<std::string, PyObjectPtr> &Scope::get_scope(const std::string& name)
{
    return scopes.at(name);
}

PyObjectPtr Scope::get_var(const std::string& var, const std::string& scope) const
{
    // Parse scope
    if (scopes.count(scope) && scopes.at(scope).count(var))
        return scopes.at(scope).at(var);

    std::size_t prev_pos = scope.length();
    while (true) {
        std::size_t new_pos = scope.find_last_of('.', prev_pos);
        if (new_pos == scope.npos)
            break;

        std::string sub = scope.substr(0, new_pos);
        if (scopes.count(sub) && scopes.at(sub).count(var))
            return scopes.at(sub).at(var);

        prev_pos = new_pos-1;
    }

    TB.raise("name '" + var + "' is not defined", "NameError");

    // This should never be reached
    return helpers::new_none();
}

void Scope::set_var(const std::string& var, const std::string& scope, PyObjectPtr value)
{
    scopes[scope][var] = value;
}