#include "String.hpp"

namespace ke
{

void toLower(std::string& str)
{
    std::transform(str.begin(), str.end(),str.begin(), tolower);
}

void toUpper(std::string& str)
{
    std::transform(str.begin(), str.end(),str.begin(), toupper);
}

std::string split(std::string& base, char separator)
{
    std::size_t found = base.find(separator);
    if (found != std::string::npos)
    {
        std::string t = base.substr(0, found);
        base = base.substr(found + 1);
        return t;
    }
    return "";
}

std::string split(std::string& base, std::string const& separator)
{
    std::size_t found = base.find(separator);
    if (found != std::string::npos)
    {
        std::string t = base.substr(0, found);
        base = base.substr(found + 1);
        return t;
    }
    return "";
}

std::vector<std::string> splitVector(std::string str, std::string const& separator)
{
    std::vector<std::string> v;
    std::size_t found;
    do
    {
        found = str.find(separator);
        if (found != std::string::npos)
        {
            v.push_back(str.substr(0,found));
            str = str.substr(found + 1);
        }
    } while (found != std::string::npos);
    v.push_back(str);
    return v;
}

bool contains(std::string const& str, char c)
{
    return str.find(c) != std::string::npos;
}

bool contains(std::string const& str, std::string const& c)
{
    return str.find(c) != std::string::npos;
}

std::string limitSize(std::string str, std::size_t size)
{
    if (str.size() <= size)
    {
        return str;
    }
    else
    {
        return str.substr(0,size);
    }
}

} // namespace ke