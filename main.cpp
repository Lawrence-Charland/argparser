#include <iostream>

#include <argparser.hpp>

using namespace argparser;

int main(int argc, char **argv) // app -v -i -c pos1 pos2
{
    // clang-format off
    ap_instance app({
        ap_entry(
            "head",
            ap_type::positional,
            {
                {ap_defkey::group,"head"},
                {ap_defkey::max_positional_count,"1"},
                {ap_defkey::min_positional_count,"1"}
            }
        ),
        ap_entry(
            "subA-input",
            ap_type::option,
            {
                {ap_defkey::group,"subA"},
                {ap_defkey::short_name,"i"},
                {ap_defkey::max_value_count,"1"},
                {ap_defkey::min_value_count,"1"}
            }
        ),
        ap_entry(
            "subA-output",
            ap_type::option,
            {
                {ap_defkey::group,"subA"},
                {ap_defkey::short_name,"o"},
                {ap_defkey::max_value_count,"1"},
                {ap_defkey::min_value_count,"1"}
            }
        ),
        ap_entry(
            "subB-verbose",
            ap_type::flag,
            {
                {ap_defkey::group,"subB"},
                {ap_defkey::short_name,"v,V"},
                {ap_defkey::max_repetition,"1"}
            }
        )
    });
    // clang-format on
    app.pre_parse(argc, argv);
    app.evaluate("head");
    if (!app.fine("head"))
        return 2;
    if (app.entry("head").content[0] == "subA")
    {
        app.evaluate("subA");
        if(!app.fine("subA"))
            return 1;
    }
    else if (app.entry("head").content[0] == "subB")
    {
        app.evaluate("subB");
        if(!app.fine("subB"))
            return 1;
    }
    else
    {
        return 2;
    }
    return 0;
}