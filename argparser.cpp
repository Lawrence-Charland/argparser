#include <algorithm>
#include <argparser.hpp>
#include <cstring>

ap_arg_entry::ap_arg_entry(const char *_argument, ap_style _style,
                           const char separator)
    : argument(_argument), flag_index(argument.find(separator))
{
    // Start with -
    bool is_short = (argument.find('-') == std::string::npos ? false : true);
    // Start with --
    bool is_long =
        (is_short == false
             ? false
             : (argument.find("--") == std::string::npos ? false : true));
    // Long is not short
    if (is_long == true)
        is_short = false;
    // Short option
    if (_style == ap_style::ap_unix_gnu && is_short == true)
        format = ap_arg_format::ap_short_opt;
    // Long option (or just option in some cases)
    else if ((_style == ap_style::ap_unix_gnu && is_long == true) ||
             (_style == ap_style::ap_xtoolkit && is_short == true) ||
             (_style == ap_style::ap_simple_kv &&
              flag_index != std::string::npos))
        format = ap_arg_format::ap_long_opt;
    // Positional
    else
        format = ap_arg_format::ap_positional;
}

argparser_instance::argparser_instance(int argc, char **argv, ap_style _style)
    : style(_style)
{
    for (int i = 0; i < argc; i++)
    {
        if (std::strcmp(argv[i], "--") == 0 && style != ap_style::ap_linear)
        {
            for (i++; i < argc; i++)
                args.emplace_back(
                    ap_arg_entry(argv[i], ap_style::ap_linear, '='));
            break;
        }
        args.emplace_back(ap_arg_entry(
            argv[i], (i == 0 ? ap_style::ap_linear : _style), '='));
    }
}

void argparser_instance::print_debug(std::ostream &os)
{
    for (auto &it : args)
        os << it.argument << '\t' << static_cast<int>(it.format) << '\t'
           << static_cast<int>(
                  it.flag_index == std::string::npos ? -1 : it.flag_index)
           << std::endl;
}

std::vector<std::string> argparser_instance::get_positionals()
{
    std::vector<std::string> result;
    for (auto &it : args)
        if (it.format == ap_arg_format::ap_positional)
            result.push_back(it.argument);
    return result;
}

std::string argparser_instance::get_positional(int sequence)
{
    for (auto &it : args)
        if (it.format == ap_arg_format::ap_positional)
            if (sequence == 0)
                return it.argument;
            else
                sequence--;
    return "";
}

int argparser_instance::find_flag(std::string long_flag)
{
    int weight = 0;
    for (auto &it : args)
    {
        switch (style)
        {
        case ap_style::ap_unix_gnu:
            if (it.format != ap_arg_format::ap_long_opt ||
                it.flag_index != std::string::npos)
                continue;
            if (it.argument.substr(2) == long_flag)
                weight++,it.used=true;
            break;
        case ap_style::ap_xtoolkit:
            if (it.format != ap_arg_format::ap_opt ||
                it.flag_index != std::string::npos)
                continue;
            if (it.argument.substr(1) == long_flag)
                weight++,it.used=true;
            break;
        case ap_style::ap_simple_kv:
            if (it.format != ap_arg_format::ap_opt ||
                it.flag_index != std::string::npos)
                continue;
            if (it.argument == long_flag)
                weight++,it.used=true;
            break;
        default:
            if (it.argument == long_flag)
                weight++,it.used=true;
        }
    }
    return weight;
}

int argparser_instance::find_flag(char short_flag)
{
    int weight = 0;
    if (style == ap_style::ap_unix_gnu)
    {
        for (auto &it : args)
            if (it.format == ap_arg_format::ap_short_opt &&
                it.flag_index == std::string::npos)
                weight += std::count(it.argument.begin(), it.argument.end(),
                                     short_flag);
    }
    else if (style == ap_style::ap_xtoolkit)
    {
        for (auto &it : args)
            if (it.format == ap_arg_format::ap_opt &&
                it.argument.length() == 2 && it.argument[1] == short_flag)
                weight++;
    }
    return weight;
}

int argparser_instance::find_flag(char short_flag, std::string long_flag)
{
    return find_flag(short_flag) + find_flag(long_flag);
}

std::vector<std::string>
argparser_instance::get_value(std::string long_option,
                              unsigned int num_required, int num_limit)
{
    std::vector<std::string> result;
    for (int i = 1; i < args.size(); i++)
    {
        if (args[i].format == ap_arg_format::ap_opt &&
            ((style == ap_style::ap_unix_gnu &&
              args[i].argument.find(long_option) == 2 &&
              args[i].flag_index == long_option.length() + 2) ||
             (style == ap_style::ap_xtoolkit &&
              args[i].argument.find(long_option) == 1 &&
              args[i].flag_index == long_option.length() + 1) ||
             (style == ap_style::ap_linear &&
              args[i].argument.find(long_option) == 0 &&
              args[i].flag_index == long_option.length())))
        {
            result.push_back(args[i].argument.substr(args[i].flag_index));
            if (result.size() == num_limit)
                break;
        }
        else if (args[i].format == ap_arg_format::ap_opt &&
                 ((style == ap_style::ap_unix_gnu &&
                   args[i].argument.substr(2) == long_option) ||
                  (style == ap_style::ap_xtoolkit &&
                   args[i].argument.substr(1) == long_option)))
        {

        }
    }
    return result;
}