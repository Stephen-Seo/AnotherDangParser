
#ifndef OPTION_FLAG_HPP
#define OPTION_FLAG_HPP

#include <string>

struct OptionFlag
{
    OptionFlag();
    OptionFlag(std::string optionFlag);
    OptionFlag(std::string optionFlag, int priority);

    bool operator >(const OptionFlag& other) const;

    std::string optionFlag;
    int priority;
};

#endif

