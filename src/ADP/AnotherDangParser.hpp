
#ifndef ANOTHER_DANG_PARSER_HPP
#define ANOTHER_DANG_PARSER_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <set>

#include "HelpInfo.hpp"

namespace ADP
{

class AnotherDangParser
{
public:
    AnotherDangParser();

    /// registers a flag (i.e. "-v")
    /**
        The flag must be specified without the leading dash.
    */
    void addFlag(std::string flag, std::function<void()> callback, std::string helpText = "");

    /// registers an option flag (i.e. "-o outputName")
    /**
        The flag must be specified without the leading dash.
    */
    void addOptionFlag(std::string flag, std::function<void(std::string)> callback, std::string helpText = "");

    /// registers a long flag (i.e. "--verbose")
    /**
        The flag must be specified without the leading dashes.
    */
    void addLongFlag(std::string lflag, std::function<void()> callback, std::string helpText = "");

    /// registers a long option flag (i.e. "--output=outputName")
    /**
        The flag must be specified without the leading dashes.
    */
    void addLongOptionFlag(std::string lflag, std::function<void(std::string)> callback, std::string helpText = "");

    /// aliases a flag with an existing one
    /**
        The format of all input flags is expected to be like "-o" or "--output".
        If the new flag already is registered, an exception will be thrown.
    */
    void aliasFlag(std::string existingFlag, std::string newFlag);

    /// Parses commands, executing callbacks depending on the read flags
    /**
        If stopOnInvalidInput is set to true, then parsing will immediately
        return false if invalid input is detected. Otherwise, invalid input
        will not stop parsing and will only affect the return value.

        \return False if invalid input was detected.
    */
    bool parse(int argc, char** argv, bool stopOnInvalidInput = true);

    /// Prints info on all flags
    void printHelp(std::ostream& ostream = std::cout);

private:
    template <typename T, typename... Args>
    struct CallbackHolder
    {
        CallbackHolder() = default;
        CallbackHolder(std::function<T(Args...)> callback, std::string helpText) :
        callback(callback),
        helpText(helpText)
        {}

        std::function<T(Args...)> callback;
        std::string helpText;
    };

    std::unordered_map<std::string, CallbackHolder<void> > callbacks;
    std::unordered_map<std::string, CallbackHolder<void, std::string> > optionCallbacks;
    std::unordered_map<std::string, CallbackHolder<void> > longCallbacks;
    std::unordered_map<std::string, CallbackHolder<void, std::string> > longOptionCallbacks;
    std::unordered_map<std::string, std::string> aliases;
    std::unordered_map<std::string, std::string> optionAliases;
    std::unordered_map<std::string, std::string> longAliases;
    std::unordered_map<std::string, std::string> longOptionAliases;
    static const std::regex dashRegex;
    static const std::regex dashFullRegex;
    static const std::regex longRegex;
    static const std::regex longFullRegex;
    std::set<HelpInfo> helpCache;
    bool isDirty;

};

} // namespace ADP

#endif

