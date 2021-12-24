#include "pyfunction.h"
#include "pytraceback.h"
using namespace cpy;
#include <algorithm>

FunctionArgument::FunctionArgument(PyObjectPtr obj, const std::string& name) : o(obj), name(name) {}

bool FunctionArgument::is_named() const
{
    return name != "";
}

std::string FunctionArgument::get_name() const 
{
    return name;
}

PyObjectPtr FunctionArgument::get_object() const
{
    return o;
}

ParsedFunctionArguments::ParsedFunctionArguments(
    const std::vector<std::string>& param_names,
    const std::vector<PyObjectPtr> defaults,
    std::size_t defaults_offset,
    std::size_t star_pos,
    bool dstar
) : param_names(param_names), defaults(defaults), 
    defaults_offset(defaults_offset), star_pos(star_pos), 
    dstar(dstar)
{}

ParseResult ParsedFunctionArguments::parse(const FunctionArguments& fa)
{
    unparsed_args = fa;
    std::size_t extra_params = 0;
    std::size_t total_positional = param_names.size() - (int) dstar - (int) (star_pos != -1);

    // FIXME: this looks a bit iffy
    for (std::size_t i = 0; i < defaults.size(); i++) {
        std::size_t k = defaults_offset + i;
        if (k >= star_pos && star_pos >= defaults_offset)
            k++;
        positional[param_names[k]] = defaults[i];
    }

    for (std::size_t i = 0; i < fa.size(); i++) {
        if (fa[i].is_named()) {
            if (i != star_pos && (i != param_names.size()-1 || !dstar) && std::any_of(param_names.begin(), param_names.end(), [&fa, i](const std::string& elem){ return elem == fa[i].get_name(); }))
                positional[fa[i].get_name()] = fa[i].get_object();
            else if (dstar) {
                kwargs[fa[i].get_name()] = fa[i].get_object();
            }
            else {
                ParseResult pr;
                pr.type = ParseResultType::UNEXPECTED_KEYWORD;
                pr.keyword = fa[i].get_name();
                return pr;
            }
        }
        else if (i < star_pos) {
            if (star_pos == -1) {
                if (i < param_names.size() - (int) dstar) {
                    positional[param_names[i]] = fa[i].get_object();
                }
                else {
                    extra_params++;
                }
            }
            else {
                positional[param_names[i]] = fa[i].get_object();
            }
        }
        else {
            args.push_back(fa[i].get_object());
        }
    }
    if (extra_params > 0) {
        ParseResult pr;
        pr.type = ParseResultType::TOO_MANY_POSITIONAL_ARGS;
        pr.needed_given = std::make_pair(total_positional, total_positional+extra_params);
        return pr;
    }
    if (positional.size() < total_positional) {
        ParseResult pr;
        pr.type = ParseResultType::MISSING_POSITIONAL_ARGS;
        for (std::size_t i = 0; i < param_names.size(); i++) {
            if (i != star_pos && (i != param_names.size()-1 || !dstar) && !positional.count(param_names[i]))
                pr.missing_args.push_back(param_names[i]);
        }
        return pr;
    }
    ParseResult pr;
    pr.type = ParseResultType::SUCCESS;
    return pr;
}

PyObjectPtr ParsedFunctionArguments::get_arg_named(const std::string& name) const
{
    return positional.at(name);
}

const std::vector<PyObjectPtr>& ParsedFunctionArguments::get_args() const
{
    return args;
}

const std::map<std::string, PyObjectPtr>& ParsedFunctionArguments::get_kwargs() const
{
    return kwargs;
}

const FunctionArguments& ParsedFunctionArguments::as_function_args() const
{
    return unparsed_args;
}

PyFunction::PyFunction(
    std::function<PyObjectPtr(const ParsedFunctionArguments&)> f, 
    const std::string& fn,
    const std::vector<std::string>& param_names,
    std::size_t star_pos,
    bool dstar
) : internal(f), function_name(fn), param_names(param_names), star_pos(star_pos), dstar(dstar) {}

PyFunction::PyFunction(
    std::function<PyObjectPtr(const ParsedFunctionArguments&)> f, 
    const std::string& fn,
    const std::vector<std::string>& param_names,
    const std::vector<PyObjectPtr> defaults,
    std::size_t defaults_offset,
    std::size_t star_pos,
    bool dstar
) : internal(f), function_name(fn), param_names(param_names), defaults(defaults), defaults_offset(defaults_offset), star_pos(star_pos), dstar(dstar) {}

PyObjectPtr PyFunction::call(const FunctionArguments &args)
{
    globals::Traceback::the().push(function_name);
    ParsedFunctionArguments parsed(
        param_names,
        defaults,
        defaults_offset,
        star_pos,
        dstar
    );
    auto parse_result = parsed.parse(args);
    
    switch (parse_result.type) {
        case ParseResultType::UNEXPECTED_KEYWORD: {
            globals::Traceback::the().raise(
                "TypeError: " + function_name + "() got an unexpected keyword argument '" + parse_result.keyword + "'", 
                "TypeError"
            );
        }
        case ParseResultType::MISSING_POSITIONAL_ARGS: {
            std::string error_msg = "TypeError: " + function_name + "() missing " + std::to_string(parse_result.missing_args.size()) + " required positional argument";
            if (parse_result.missing_args.size() != 1)
                error_msg += "s";
            error_msg += ": ";

            for (std::size_t i = 0; i < parse_result.missing_args.size(); i++) {
                error_msg += "'" + parse_result.missing_args[i] + "'";
                if (i != parse_result.missing_args.size()-1 && parse_result.missing_args.size() > 2)
                    error_msg += ",";
                if (i == parse_result.missing_args.size()-2)
                    error_msg += " and";
                error_msg += " ";
            }

            globals::Traceback::the().raise(
                error_msg,
                "TypeError"
            );
        }
        case ParseResultType::TOO_MANY_POSITIONAL_ARGS: {
            globals::Traceback::the().raise(
                "TypeError: " + function_name + "() takes " + std::to_string(parse_result.needed_given.first) + " positional arguments but " + std::to_string(parse_result.needed_given.second) + " were given",
                "TypeError"
            );
        }
        default:
            break;
    }

    auto result = internal(parsed);
    globals::Traceback::the().pop();
    return result;
}