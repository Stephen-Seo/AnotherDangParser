
#include "AnotherDangParser.hpp"

#include <iostream>
#include <cstdlib>
#include <cstring>

int main()
{
    std::cout << "Simulating input with input:\n\t./ParserTest -daop Option_O Option_P\n";

    int argc = 4;
    char* argv[5];

    argv[0] = (char*)std::malloc(sizeof(char) * 13);
    argv[1] = (char*)std::malloc(sizeof(char) * 6);
    argv[2] = (char*)std::malloc(sizeof(char) * 9);
    argv[3] = (char*)std::malloc(sizeof(char) * 9);
    argv[4] = (char*)std::malloc(sizeof(char) * 1);

    argv[0][0] = '.';
    argv[0][1] = '/';
    argv[0][2] = 'P';
    argv[0][3] = 'a';
    argv[0][4] = 'r';
    argv[0][5] = 's';
    argv[0][6] = 'e';
    argv[0][7] = 'r';
    argv[0][8] = 'T';
    argv[0][9] = 'e';
    argv[0][10] = 's';
    argv[0][11] = 't';
    argv[0][12] = '\0';

    argv[1][0] = '-';
    argv[1][1] = 'd';
    argv[1][2] = 'a';
    argv[1][3] = 'o';
    argv[1][4] = 'p';
    argv[1][5] = '\0';

    argv[2][0] = 'O';
    argv[2][1] = 'p';
    argv[2][2] = 't';
    argv[2][3] = 'i';
    argv[2][4] = 'o';
    argv[2][5] = 'n';
    argv[2][6] = '_';
    argv[2][7] = 'O';
    argv[2][8] = '\0';

    std::strcpy(argv[3], "Option_P\0");
    std::strcpy(argv[4], "\0");

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

    for(int i = 0; i < 5; ++i)
    {
        free(argv[i]);
    }
}
