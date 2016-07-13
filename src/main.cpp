
#include "AnotherDangParser.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
    AnotherDangParser p;

    p.addFlag("v", [] () {
        std::cout << "-v detected." << std::endl;
    });

    p.addOptionFlag("o", [] (std::string o) {
        std::cout << "got o with param " << o << '.' << std::endl;
    });

    p.addLongFlag("derp", [] () {
        std::cout << "--derp detected" << std::endl;
    });

    p.addLongOptionFlag("test", [] (std::string in) {
        std::cout << "(test) " << in << '.' << std::endl;
    });

    p.aliasFlag("-o", "--option");
    p.aliasFlag("-v", "--verbose");
    p.aliasFlag("-o", "-i");
    p.aliasFlag("-v", "-x");
    p.aliasFlag("--derp", "--anotherDerp");
    p.aliasFlag("--derp", "-d");
    p.aliasFlag("--test", "--altTest");
    p.aliasFlag("--test", "-t");

    if(argc > 1)
    {
        p.parse(argc, argv);
    }
    else
    {
        p.printHelp();
    }
}

