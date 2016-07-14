
#ifndef OPTION_FLAG_HPP
#define OPTION_FLAG_HPP

#include <string>

namespace ADP
{

struct OptionFlag
{
    OptionFlag();
    OptionFlag(std::string optionFlag);
    OptionFlag(std::string optionFlag, int priority);

    bool operator >(const OptionFlag& other) const;

    std::string optionFlag;
    int priority;
};

} // namespace ADP

#endif

