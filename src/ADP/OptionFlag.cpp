
#include "OptionFlag.hpp"

using namespace ADP;

OptionFlag::OptionFlag() :
optionFlag(),
priority(0)
{}

OptionFlag::OptionFlag(std::string optionFlag) :
optionFlag(optionFlag),
priority(0)
{}

OptionFlag::OptionFlag(std::string optionFlag, int priority) :
optionFlag(optionFlag),
priority(priority)
{}

bool OptionFlag::operator >(const OptionFlag& other) const
{
    return priority > other.priority;
}
