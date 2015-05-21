
#include "AnotherDangParser.hpp"

int main(int argc, char** argv)
{
    AnotherDangParser parser;
    parser.addFlag("d", [] () {
        std::cout << "Derp.\n";
    });
    parser.addFlag("a", [] () {
        std::cout << "Apps.\n";
    });

    parser.parse(argc, argv);
}
