#ifndef _PYWRAPPERFUNCTION_H_
#define _PYWRAPPERFUNCTION_H_

#include <functional>
#include <vector>
#include <map>
#include <string>
#include "pyobject.h"

namespace cppy {
    class FunctionArgument {
    public:
        FunctionArgument() = default;
        FunctionArgument(PyObjectPtr, const std::string& name = "");
        bool is_named() const;
        PyObjectPtr get_object() const;
        std::string get_name() const; 

    private:
        PyObjectPtr o;
        std::string name;
    };

    class FunctionArguments : public std::vector<FunctionArgument> {
    public:
        using std::vector<FunctionArgument>::vector;
        void parse(
            const std::string& scope,
            const std::vector<std::string>& param_names,
            const std::vector<PyObjectPtr> defaults,
            std::size_t defaults_offset,
            std::size_t star_pos = -1,
            bool dstar = false
        ) const;
    };

    enum class ParseResultType {
        SUCCESS,
        TOO_MANY_POSITIONAL_ARGS,
        MISSING_POSITIONAL_ARGS,
        UNEXPECTED_KEYWORD
    };

    struct ParseResult {
        // This should really be a union
        ParseResultType type;
        std::vector<std::string> missing_args;
        std::pair<std::size_t, std::size_t> needed_given;
        std::string keyword;
    };

    class ParsedFunctionArguments {
    public:
        ParsedFunctionArguments(
            const std::vector<std::string>& param_names,
            const std::vector<PyObjectPtr> defaults,
            std::size_t defaults_offset,
            std::size_t star_pos = -1,
            bool dstar = false
        );

        ParseResult parse(const FunctionArguments&);

        PyObjectPtr get_arg_named(const std::string& name) const;
        bool has_arg_named(const std::string& name) const;
        const std::vector<PyObjectPtr>& get_args() const;
        const std::map<std::string, PyObjectPtr>& get_kwargs() const;
        const FunctionArguments& as_function_args() const;

    private:
        std::vector<std::string> param_names;
        std::vector<PyObjectPtr> defaults;
        std::size_t defaults_offset, star_pos;
        bool dstar;
        FunctionArguments unparsed_args;

        std::map<std::string, PyObjectPtr> positional, kwargs;
        std::vector<PyObjectPtr> args;
    };

    class PyFunction : public PyObject {
    public:
        PyFunction(
            std::function<PyObjectPtr(const ParsedFunctionArguments&)>, 
            const std::string& name
        );
        PyFunction(
            std::function<PyObjectPtr(const ParsedFunctionArguments&)>, 
            const std::string& name, 
            const std::vector<std::string>& param_names,
            std::size_t star_pos = -1,
            bool dstar = false
        );
        PyFunction(
            std::function<PyObjectPtr(const ParsedFunctionArguments&)>, 
            const std::string& name, 
            const std::vector<std::string>& param_names,
            const std::vector<PyObjectPtr> defaults,
            std::size_t defaults_offset,
            std::size_t star_pos = -1,
            bool dstar = false
        );
        ~PyFunction() = default;

        PyObjectPtr call(const FunctionArguments&);
        static PyObjectPtr call_wrapper(const PyObject&);
        static PyObjectPtr __repr__;
        std::string function_name;

    private:
        std::function<PyObjectPtr(const ParsedFunctionArguments&)> internal;
        std::vector<std::string> param_names;
        std::vector<PyObjectPtr> defaults;
        std::size_t defaults_offset, star_pos;
        bool dstar = false;
    };

    typedef std::function<PyObjectPtr(const ParsedFunctionArguments&)> PyInternalFunc;
}

#endif