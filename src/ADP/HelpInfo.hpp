
#ifndef HELP_INFO
#define HELP_INFO

#include <string>
#include <vector>

namespace ADP
{

struct HelpInfo
{
    HelpInfo() = default;
    HelpInfo(std::string flag, bool isLong, bool requiresOption, std::string helpText);
    HelpInfo(std::string flag, bool isLong, bool requiresOption, std::string helpText, std::vector<std::string> aliases);

    std::string flag;
    bool isLong;
    bool requiresOption;
    std::string helpText;
    std::vector<std::string> aliases;

    bool operator <(const HelpInfo& other) const;
};

} // namespace ADP

#endif

