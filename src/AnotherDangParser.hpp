
#include <iostream>
#include <cstring>
#include <functional>
#include <string>
#include <map>
#include <regex>

class AnotherDangParser
{
public:
    AnotherDangParser();
    ~AnotherDangParser();

    void addFlag(std::string flag, std::function<void()> callback);
    void parse(int argc, char** argv, bool ignoreFirstParameter = true);
private:
    std::map<std::string, std::function<void()> > callbacks;
    std::regex dashRegex;

};

