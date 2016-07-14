
#include "HelpInfo.hpp"

using namespace ADP;

HelpInfo::HelpInfo(std::string flag, bool isLong, bool requiresOption, std::string helpText) :
flag(flag),
isLong(isLong),
requiresOption(requiresOption),
helpText(helpText)
{}

HelpInfo::HelpInfo(std::string flag, bool isLong, bool requiresOption, std::string helpText, std::vector<std::string> aliases) :
flag(flag),
isLong(isLong),
requiresOption(requiresOption),
helpText(helpText),
aliases(aliases)
{}

bool HelpInfo::operator <(const HelpInfo& other) const
{
    return flag < other.flag;
}
