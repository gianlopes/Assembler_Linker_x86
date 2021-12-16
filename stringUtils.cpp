#include "stringUtils.hpp"

bool validIdentifier(std::string s)
{
    std::regex e{"^[a-zA-Z_][a-zA-Z_0-9]*$"};
    if (!std::regex_match(s, e))
        return false;
    else if (s.size() > 50)
        return false;
    return true;
}

std::vector<std::string> tokenize(std::string s, std::string delimiter)
{
    std::vector<std::string> vec;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        vec.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if (s != "")
        vec.push_back(s);
    return vec;
}

std::string strToLower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

// trim from start
void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end
void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(),
            s.end());
}

// trim from both ends
void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}