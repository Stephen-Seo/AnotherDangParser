
#include "AnotherDangParser.hpp"
#include "OptionFlag.hpp"

#include <cstring>
#include <queue>
#include <vector>
#include <stdexcept>
#include <set>

const std::regex AnotherDangParser::dashRegex("^-[^-].*$");
const std::regex AnotherDangParser::dashFullRegex("^-([a-zA-Z])$");
const std::regex AnotherDangParser::longRegex("^--([a-zA-Z]+)$");
const std::regex AnotherDangParser::longFullRegex("^--([a-zA-Z]+)=(.+)$");

void AnotherDangParser::addFlag(std::string flag, std::function<void()> callback)
{
    callbacks.insert(std::make_pair(flag, callback));
}

void AnotherDangParser::addOptionFlag(std::string flag, std::function<void(std::string)> callback)
{
    optionCallbacks.insert(std::make_pair(flag, callback));
}

void AnotherDangParser::addLongFlag(std::string lflag, std::function<void()> callback)
{
    longCallbacks.insert(std::make_pair(lflag, callback));
}

void AnotherDangParser::addLongOptionFlag(std::string lflag, std::function<void(std::string)> callback)
{
    longOptionCallbacks.insert(std::make_pair(lflag, callback));
}

void AnotherDangParser::aliasFlag(std::string existingFlag, std::string newFlag)
{
    std::smatch eMatch, nMatch;
    bool eIsValid = false;
    bool nIsValid = false;
    bool eIsLong, nIsLong;

    if(std::regex_match(existingFlag, eMatch, AnotherDangParser::dashFullRegex))
    {
        eIsValid = true;
        eIsLong = false;
    }
    else if(std::regex_match(existingFlag, eMatch, AnotherDangParser::longRegex))
    {
        eIsValid = true;
        eIsLong = true;
    }
    if(std::regex_match(newFlag, nMatch, AnotherDangParser::dashFullRegex))
    {
        nIsValid = true;
        nIsLong = false;
    }
    else if(std::regex_match(newFlag, nMatch, AnotherDangParser::longRegex))
    {
        nIsValid = true;
        nIsLong = true;
    }

    if(!eIsValid)
    {
        throw std::invalid_argument("Existing flag is invalid!");
    }
    else if(!nIsValid)
    {
        throw std::invalid_argument("New flag is invalid!");
    }

    if(callbacks.find(nMatch[1]) != callbacks.end() ||
        optionCallbacks.find(nMatch[1]) != optionCallbacks.end() ||
        longCallbacks.find(nMatch[1]) != longCallbacks.end() ||
        longOptionCallbacks.find(nMatch[1]) != longOptionCallbacks.end() ||
        aliases.find(nMatch[1]) != aliases.end() ||
        optionAliases.find(nMatch[1]) != optionAliases.end())
    {
        throw std::invalid_argument("New flag already exists!");
    }

    bool eIsOption;
    if(eIsLong)
    {
        auto eiter = longCallbacks.find(eMatch[1]);
        auto eoiter = longOptionCallbacks.find(eMatch[1]);
        if(eiter != longCallbacks.end())
        {
            eIsOption = false;
        }
        else if(eoiter != longOptionCallbacks.end())
        {
            eIsOption = true;
        }
        else
        {
            throw std::invalid_argument("Existing flag does not exist!");
        }
    }
    else
    {
        auto eiter = callbacks.find(eMatch[1]);
        auto eoiter = optionCallbacks.find(eMatch[1]);
        if(eiter != callbacks.end())
        {
            eIsOption = false;
        }
        else if(eoiter != optionCallbacks.end())
        {
            eIsOption = true;
        }
        else
        {
            throw std::invalid_argument("Existing flag does not exist!");
        }
    }

    if(eIsOption)
    {
        if(nIsLong)
        {
            longOptionAliases.insert(std::make_pair(nMatch[1], eMatch[1]));
        }
        else
        {
            optionAliases.insert(std::make_pair(nMatch[1], eMatch[1]));
        }
    }
    else
    {
        if(nIsLong)
        {
            longAliases.insert(std::make_pair(nMatch[1], eMatch[1]));
        }
        else
        {
            aliases.insert(std::make_pair(nMatch[1], eMatch[1]));
        }
    }
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
        if(optionFlagQueue.empty())
        {
            if(std::regex_match(argv[0], AnotherDangParser::dashRegex))
            {
                unsigned int i = 0;
                std::string flag;
                for(char* arg = argv[0]; arg[0] != '\0'; ++arg)
                {
                    flag.assign(1, arg[0]);
                    auto iter = callbacks.find(flag);
                    auto oiter = optionCallbacks.find(flag);
                    auto aiter = aliases.find(flag);
                    auto oaiter = optionAliases.find(flag);
                    if(iter != callbacks.end())
                    {
                        iter->second();
                    }
                    else if(oiter != optionCallbacks.end())
                    {
                        optionFlagQueue.push(OptionFlag(flag, i++));
                    }
                    else if(aiter != aliases.end())
                    {
                        auto a2c = callbacks.find(aiter->second);
                        auto a2lc = longCallbacks.find(aiter->second);
                        if(a2c != callbacks.end())
                        {
                            a2c->second();
                        }
                        else if(a2lc != longCallbacks.end())
                        {
                            a2lc->second();
                        }
                    }
                    else if(oaiter != optionAliases.end())
                    {
                        auto oa2o = optionCallbacks.find(oaiter->second);
                        auto oa2lo = longOptionCallbacks.find(oaiter->second);
                        if(oa2o != optionCallbacks.end())
                        {
                            optionFlagQueue.push(OptionFlag(oa2o->first, i++));
                        }
                        else if(oa2lo != longOptionCallbacks.end())
                        {
                            optionFlagQueue.push(OptionFlag(oa2lo->first, i++));
                        }
                    }
                }
            }
            else
            {
                std::cmatch match;
                if(std::regex_match(argv[0], match, AnotherDangParser::longRegex))
                {
                    std::string longName = match[1];
                    auto iter = longCallbacks.find(longName);
                    auto laiter = longAliases.find(longName);
                    if(iter != longCallbacks.end())
                    {
                        iter->second();
                    }
                    else if(laiter != longAliases.end())
                    {
                        auto la2c = callbacks.find(laiter->second);
                        auto la2lc = longCallbacks.find(laiter->second);
                        if(la2c != callbacks.end())
                        {
                            la2c->second();
                        }
                        else if(la2lc != longCallbacks.end())
                        {
                            la2lc->second();
                        }
                    }
                }
                else if(std::regex_match(argv[0], match, AnotherDangParser::longFullRegex))
                {
                    std::string longName = match[1];
                    auto iter = longOptionCallbacks.find(longName);
                    auto loaiter = longOptionAliases.find(longName);
                    if(iter != longOptionCallbacks.end())
                    {
                        iter->second(match[2]);
                    }
                    else if(loaiter != longOptionAliases.end())
                    {
                        auto loa2oc = optionCallbacks.find(loaiter->second);
                        auto loa2loc = longOptionCallbacks.find(loaiter->second);
                        if(loa2oc != optionCallbacks.end())
                        {
                            loa2oc->second(match[2]);
                        }
                        else if(loa2loc != longOptionCallbacks.end())
                        {
                            loa2loc->second(match[2]);
                        }
                    }
                }
            }
        }
        else
        {
            OptionFlag of = optionFlagQueue.top();
            optionFlagQueue.pop();

            auto oc = optionCallbacks.find(of.optionFlag);
            auto loc = longOptionCallbacks.find(of.optionFlag);
            if(oc != optionCallbacks.end())
            {
                oc->second(std::string(argv[0]));
            }
            else if(loc != longOptionCallbacks.end())
            {
                loc->second(std::string(argv[0]));
            }
            else
            {
                throw std::runtime_error("Invalid flag in OptionQueue!");
            }
        }

        --argc;
        ++argv;
    }
}

void AnotherDangParser::printHelp(std::ostream& ostream)
{
}

