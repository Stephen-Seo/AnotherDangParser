
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
