
#include "AnotherDangParser.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    AnotherDangParser parser;
    parser.addFlag("d", [] () {
        std::cout << "Derp.\n";
    });
    parser.addFlag("a", [] () {
        std::cout << "Apps.\n";
    });

    parser.addOptionFlag("o", [] (std::string option) {
        std::cout << "Option: " << option << '\n';
    });
    parser.addOptionFlag("p", [] (std::string option) {
        std::cout << "p Option: " << option << '\n';
    });

    parser.parse(argc, argv);
}
