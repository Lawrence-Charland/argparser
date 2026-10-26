#include <iostream>

#include <argparser.hpp>

int main(int argc, char **argv)
{
    argparser_instance arg(argc,argv,ap_style::ap_xtoolkit);
    arg.print_debug(std::cout);
    std::cout<<std::endl;
    std::cout<<arg.find_flag('v',"verbose")<<std::endl;
    return 0;
}