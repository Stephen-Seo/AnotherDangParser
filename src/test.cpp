
#include <ADP/AnotherDangParser.hpp>

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

int main(int argc, char** argv)
{
    using namespace ADP;

    AnotherDangParser p;

    try {
        p.addFlag("-", [] () {});
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid flag 0" << std::endl;
    }

    try {
        p.addOptionFlag("-", [] (std::string unused) {});
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid flag 1" << std::endl;
    }

    try {
        p.addLongFlag("-", [] () {});
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid flag 2" << std::endl;
    }

    try {
        p.addLongOptionFlag("-", [] (std::string unused) {});
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid flag 3" << std::endl;
    }

    try {
        p.aliasFlag("-d", "--this-doesnt-matter");
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid alias 0 " << e.what() << std::endl;
    }

    try {
        p.aliasFlag("--does-not-exist", "--this-doesnt-matter");
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid alias 1 " << e.what() << std::endl;
    }

    try {
        p.aliasFlag("-", "--this-doesnt-matter");
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid alias 2 " << e.what() << std::endl;
    }

    try {
        p.aliasFlag("--this-doesnt-matter", "-");
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid alias 3 " << e.what() << std::endl;
    }

    p.addFlag("v", [] () {
        std::cout << "-v detected." << std::endl;
    });

    try {
        p.aliasFlag("-v", "-v");
    } catch (std::invalid_argument& e) {
        std::cout << "Caught invalid alias 4 " << e.what() << std::endl;
    }

    p.addOptionFlag("o", [] (std::string o) {
        std::cout << "got o with param " << o << '.' << std::endl;
    });

    p.addLongFlag("derp", [] () {
        std::cout << "--derp detected" << std::endl;
    });

    p.addLongOptionFlag("test", [] (std::string in) {
        std::cout << "(test) " << in << '.' << std::endl;
    });

    p.addLongFlag("dash-and_underscore", [] () {
        std::cout << "--dash-and_underscore detected" << std::endl;
    });

    p.addLongFlag("s", [] () {
        std::cout << "just s detected" << std::endl;
    },
    "just s");

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
        if(!p.parse(argc, argv))
        {
            std::cout << "Got invalid input, printing help..." << std::endl;
            p.printHelp();
        }
    }
    else
    {
        p.printHelp();
    }
}

