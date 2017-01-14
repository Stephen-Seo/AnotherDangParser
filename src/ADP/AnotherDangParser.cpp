
#include "AnotherDangParser.hpp"

#include <cstring>
#include <queue>
#include <vector>
#include <stdexcept>

#include "OptionFlag.hpp"

using namespace ADP;

const std::regex AnotherDangParser::dashRegex("^-[^-].*$");
const std::regex AnotherDangParser::dashFullRegex("^-([a-zA-Z])$");
const std::regex AnotherDangParser::longRegex("^--([a-zA-Z][a-zA-Z_-]*)$");
const std::regex AnotherDangParser::longFullRegex("^--([a-zA-Z][a-zA-Z_-]*)=(.+)$");

AnotherDangParser::AnotherDangParser() :
isDirty(true)
{}

void AnotherDangParser::addFlag(std::string flag, std::function<void()> callback, std::string helpText)
{
    if(!checkIsValidShort("-" + flag))
    {
        throw std::invalid_argument("Invalid short flag cannot be registered!");
    }

    callbacks.insert(std::make_pair(flag, CallbackHolder<void>(callback, helpText)));
    isDirty = true;
}

void AnotherDangParser::addOptionFlag(std::string flag, std::function<void(std::string)> callback, std::string helpText)
{
    if(!checkIsValidShort("-" + flag))
    {
        throw std::invalid_argument("Invalid short flag cannot be registered!");
    }

    optionCallbacks.insert(std::make_pair(flag, CallbackHolder<void, std::string>(callback, helpText)));
    isDirty = true;
}

void AnotherDangParser::addLongFlag(std::string lflag, std::function<void()> callback, std::string helpText)
{
    if(!checkIsValidLong("--" + lflag))
    {
        throw std::invalid_argument("Invalid long flag cannot be registered!");
    }

    longCallbacks.insert(std::make_pair(lflag, CallbackHolder<void>(callback, helpText)));
    isDirty = true;
}

void AnotherDangParser::addLongOptionFlag(std::string lflag, std::function<void(std::string)> callback, std::string helpText)
{
    if(!checkIsValidLong("--" + lflag))
    {
        throw std::invalid_argument("Invalid long flag cannot be registered!");
    }

    longOptionCallbacks.insert(std::make_pair(lflag, CallbackHolder<void, std::string>(callback, helpText)));
    isDirty = true;
}

void AnotherDangParser::aliasFlag(std::string existingFlag, std::string newFlag)
{
    std::smatch eMatch, nMatch;
    bool eIsLong, nIsLong;

    if(std::regex_match(existingFlag, eMatch, AnotherDangParser::dashFullRegex))
    {
        eIsLong = false;
    }
    else if(std::regex_match(existingFlag, eMatch, AnotherDangParser::longRegex))
    {
        eIsLong = true;
    }
    else
    {
        throw std::invalid_argument("Existing flag is invalid!");
    }

    if(std::regex_match(newFlag, nMatch, AnotherDangParser::dashFullRegex))
    {
        nIsLong = false;
    }
    else if(std::regex_match(newFlag, nMatch, AnotherDangParser::longRegex))
    {
        nIsLong = true;
    }
    else
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
    isDirty = true;
}

bool AnotherDangParser::parse(int argc, char** argv, bool stopOnInvalidInput)
{
    // ignore first parameter which is typically the name of the program
    // with the relative/abosulte path to it
    --argc;
    ++argv;

    bool gotInvalidInput = false;

    std::priority_queue<OptionFlag, std::vector<OptionFlag>, std::greater<OptionFlag> > optionFlagQueue;
    while(argc > 0)
    {
        if(optionFlagQueue.empty())
        {
            if(std::regex_match(argv[0], AnotherDangParser::dashRegex))
            {
                unsigned int i = 0;
                std::string flag;
                // iterate through all short flags clumped together
                for(char* arg = argv[0] + 1; arg[0] != '\0'; ++arg)
                {
                    flag.assign(1, arg[0]);
                    auto iter = callbacks.find(flag);
                    auto oiter = optionCallbacks.find(flag);
                    auto aiter = aliases.find(flag);
                    auto oaiter = optionAliases.find(flag);
                    if(iter != callbacks.end())
                    {
                        iter->second.callback();
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
                            a2c->second.callback();
                        }
                        else if(a2lc != longCallbacks.end())
                        {
                            a2lc->second.callback();
                        }
                        else
                        {
                            throw std::runtime_error("ERROR: Failed to find matching alias!");
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
                        else
                        {
                            throw std::runtime_error("ERROR: Failed to find matching alias!");
                        }
                    }
                    else
                    {
#ifndef NDEBUG
                        std::cout << "Got invalid input with " << flag << std::endl;
#endif
                        if(stopOnInvalidInput)
                        {
                            return false;
                        }
                        gotInvalidInput = true;
                    }
                }
            }
            else // did not match dashRegex
            {
                std::cmatch match;
                if(std::regex_match(argv[0], match, AnotherDangParser::longRegex))
                {
                    std::string longName = match[1];
                    auto iter = longCallbacks.find(longName);
                    auto laiter = longAliases.find(longName);
                    if(iter != longCallbacks.end())
                    {
                        iter->second.callback();
                    }
                    else if(laiter != longAliases.end())
                    {
                        auto la2c = callbacks.find(laiter->second);
                        auto la2lc = longCallbacks.find(laiter->second);
                        if(la2c != callbacks.end())
                        {
                            la2c->second.callback();
                        }
                        else if(la2lc != longCallbacks.end())
                        {
                            la2lc->second.callback();
                        }
                        else
                        {
                            throw std::runtime_error("ERROR: Failed to find matching alias!");
                        }
                    }
                    else
                    {
#ifndef NDEBUG
                        std::cout << "Got invalid input with " << longName << std::endl;
#endif
                        if(stopOnInvalidInput)
                        {
                            return false;
                        }
                        gotInvalidInput = true;
                    }
                }
                else if(std::regex_match(argv[0], match, AnotherDangParser::longFullRegex))
                {
                    std::string longName = match[1];
                    auto iter = longOptionCallbacks.find(longName);
                    auto loaiter = longOptionAliases.find(longName);
                    if(iter != longOptionCallbacks.end())
                    {
                        iter->second.callback(match[2]);
                    }
                    else if(loaiter != longOptionAliases.end())
                    {
                        auto loa2oc = optionCallbacks.find(loaiter->second);
                        auto loa2loc = longOptionCallbacks.find(loaiter->second);
                        if(loa2oc != optionCallbacks.end())
                        {
                            loa2oc->second.callback(match[2]);
                        }
                        else if(loa2loc != longOptionCallbacks.end())
                        {
                            loa2loc->second.callback(match[2]);
                        }
                        else
                        {
                            throw std::runtime_error("ERROR: Failed to find matching alias!");
                        }
                    }
                    else
                    {
#ifndef NDEBUG
                        std::cout << "Got invalid input with " << longName << std::endl;
#endif
                        if(stopOnInvalidInput)
                        {
                            return false;
                        }
                        gotInvalidInput = true;
                    }
                }
                else
                {
                    if(stopOnInvalidInput)
                    {
                        return false;
                    }
                    gotInvalidInput = true;
                }
            }
        }
        else // optionFlagQueue not empty
        {
            OptionFlag of = optionFlagQueue.top();
            optionFlagQueue.pop();

            auto oc = optionCallbacks.find(of.optionFlag);
            auto loc = longOptionCallbacks.find(of.optionFlag);
            if(oc != optionCallbacks.end())
            {
                oc->second.callback(std::string(argv[0]));
            }
            else if(loc != longOptionCallbacks.end())
            {
                loc->second.callback(std::string(argv[0]));
            }
            else
            {
                throw std::runtime_error("Invalid flag in OptionQueue!");
            }
        }

        --argc;
        ++argv;
    }

    if(!optionFlagQueue.empty())
    {
#ifndef NDEBUG
        std::cout << "Got invalid input because optionFlagQueue was not empty" << std::endl;
#endif
        gotInvalidInput = true;
    }

    return !gotInvalidInput;
}

void AnotherDangParser::printHelp(std::ostream& ostream)
{
    if(isDirty)
    {
        helpCache.clear();
        for(auto iter = callbacks.begin(); iter != callbacks.end(); ++iter)
        {
            HelpInfo helpInfo(iter->first, false, false, iter->second.helpText);
            for(auto aiter = aliases.begin(); aiter != aliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("-" + aiter->first);
                }
            }
            for(auto aiter = longAliases.begin(); aiter != longAliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("--" + aiter->first);
                }
            }
            helpCache.insert(helpInfo);
        }
        for(auto iter = optionCallbacks.begin(); iter != optionCallbacks.end(); ++iter)
        {
            HelpInfo helpInfo(iter->first, false, true, iter->second.helpText);
            for(auto aiter = optionAliases.begin(); aiter != optionAliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("-" + aiter->first);
                }
            }
            for(auto aiter = longOptionAliases.begin(); aiter != longOptionAliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("--" + aiter->first);
                }
            }
            helpCache.insert(helpInfo);
        }
        for(auto iter = longCallbacks.begin(); iter != longCallbacks.end(); ++iter)
        {
            HelpInfo helpInfo(iter->first, true, false, iter->second.helpText);
            for(auto aiter = aliases.begin(); aiter != aliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("-" + aiter->first);
                }
            }
            for(auto aiter = longAliases.begin(); aiter != longAliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("--" + aiter->first);
                }
            }
            helpCache.insert(helpInfo);
        }
        for(auto iter = longOptionCallbacks.begin(); iter != longOptionCallbacks.end(); ++iter)
        {
            HelpInfo helpInfo(iter->first, true, true, iter->second.helpText);
            for(auto aiter = optionAliases.begin(); aiter != optionAliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("-" + aiter->first);
                }
            }
            for(auto aiter = longOptionAliases.begin(); aiter != longOptionAliases.end(); ++aiter)
            {
                if(aiter->second == iter->first)
                {
                    helpInfo.aliases.push_back("--" + aiter->first);
                }
            }
            helpCache.insert(helpInfo);
        }
        isDirty = false;
    }

    ostream << "Usage:\n\n";
    for(auto iter = helpCache.begin(); iter != helpCache.end(); ++iter)
    {
        if(iter->isLong)
        {
            ostream << "  --";
        }
        else
        {
            ostream << "  -";
        }

        ostream << iter->flag;

        if(!iter->aliases.empty())
        {
            ostream << "\nAliases:\n  ";
            for(auto aiter = iter->aliases.begin(); aiter != iter->aliases.end(); ++aiter)
            {
                ostream << *aiter << " ";
            }
        }

        ostream << "\n";

        if(iter->helpText.empty())
        {
            ostream << "(No help text provided.)";
        }
        else
        {
            ostream << iter->helpText;
        }

        ostream << "\nExample(s):\n";

        if(iter->isLong)
        {
            if(iter->requiresOption)
            {
                ostream << "  --" << iter->flag << "=<parameter>";
            }
            else
            {
                ostream << "  --" << iter->flag;
            }
        }
        else
        {
            if(iter->requiresOption)
            {
                ostream << "  -" << iter->flag << " <parameter>";
            }
            else
            {
                ostream << "  -" << iter->flag;
            }
        }
        ostream << '\n';

        // alias examples
        std::smatch smatch;
        for(auto aiter = iter->aliases.begin(); aiter != iter->aliases.end(); ++aiter)
        {
            if(iter->requiresOption)
            {
                if(std::regex_match(*aiter, smatch, AnotherDangParser::dashFullRegex))
                {
                    // Is short flag
                    ostream << "  " << *aiter << " <parameter>\n";
                }
                else
                {
                    // Is long flag
                    ostream << "  " << *aiter << "=<parameter>\n";
                }
            }
            else
            {
                // Either flag, similar output
                ostream << "  " << *aiter << '\n';
            }
        }

        ostream << '\n';
    }
    ostream << std::endl;
}

bool AnotherDangParser::checkIsValidShort(std::string shortFlag)
{
    if(std::regex_match(shortFlag, AnotherDangParser::dashFullRegex))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool AnotherDangParser::checkIsValidLong(std::string longFlag)
{
    if(std::regex_match(longFlag, AnotherDangParser::longRegex))
    {
        return true;
    }
    else
    {
        return false;
    }
}

