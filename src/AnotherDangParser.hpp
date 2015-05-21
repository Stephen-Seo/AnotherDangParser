
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
    void addOptionFlag(std::string flag, std::function<void(std::string)> callback);
    void parse(int argc, char** argv, bool ignoreFirstParameter = true);
private:
    std::map<std::string, std::function<void()> > callbacks;
    std::map<std::string, std::function<void(std::string)> > optionCallbacks;
    std::regex dashRegex;

};

