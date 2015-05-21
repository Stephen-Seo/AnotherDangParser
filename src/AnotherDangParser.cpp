
#include "AnotherDangParser.hpp"
#include "OptionFlag.hpp"

#include <cstring>
#include <queue>
#include <vector>

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

void AnotherDangParser::addOptionFlag(std::string flag, std::function<void(std::string)> callback)
{
    optionCallbacks.insert(std::make_pair(flag, callback));
}

void AnotherDangParser::parse(int argc, char** argv, bool ignoreFirstParameter)
{
    if(ignoreFirstParameter)
    {
        --argc;
        ++argv;
    }

    std::priority_queue<OptionFlag, std::vector<OptionFlag>, std::greater<OptionFlag> > optionFlagQueue;
    while(argc > 0)
    {
        if(optionFlagQueue.empty() && std::regex_search(argv[0], dashRegex))
        {
            for(auto cIter = callbacks.begin(); cIter != callbacks.end(); ++cIter)
            {
                std::regex regex(cIter->first);
                if(std::regex_search(argv[0], regex))
                {
                    cIter->second();
                }
            }
            for(auto ocIter = optionCallbacks.begin(); ocIter != optionCallbacks.end(); ++ocIter)
            {
                std::regex regex(ocIter->first);
                std::cmatch matchInfo;
                if(std::regex_search(argv[0], matchInfo, regex))
                {
                    optionFlagQueue.push(OptionFlag(ocIter->first, matchInfo.position()));
                }
            }
        }
        else if(!optionFlagQueue.empty())
        {
            OptionFlag of = optionFlagQueue.top();
            optionFlagQueue.pop();

            optionCallbacks.at(of.optionFlag)(std::string(argv[0]));
        }

        --argc;
        ++argv;
    }
}

