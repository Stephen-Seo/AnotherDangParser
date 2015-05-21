
#include "AnotherDangParser.hpp"

AnotherDangParser::AnotherDangParser() :
callbacks(),
dashRegex("^-")
{}

AnotherDangParser::~AnotherDangParser()
{}

void AnotherDangParser::addFlag(std::string flag, std::function<void()> callback)
{
    callbacks.insert(std::make_pair(flag, callback));
}

void AnotherDangParser::parse(int argc, char** argv, bool ignoreFirstParameter)
{
    if(ignoreFirstParameter)
    {
        --argc;
        ++argv;
    }

    while(argc > 0)
    {
        for(auto cIter = callbacks.begin(); cIter != callbacks.end(); ++cIter)
        {
            if(std::regex_search(argv[0], dashRegex))
            {
                std::regex regex(cIter->first);
                if(std::regex_search(argv[0], regex))
                {
                    cIter->second();
                }
            }
        }

        --argc;
        ++argv;
    }
}

