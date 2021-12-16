#ifndef _STRING_UTILS_HPP
#define _STRING_UTILS_HPP

#include <string>

#include <algorithm> // std::transform
#include <regex>
#include <vector>

bool validIdentifier(std::string s);
std::vector<std::string> tokenize(std::string s, std::string delimiter);
std::string strToLower(std::string s);
// trim from start
void ltrim(std::string &s);
// trim from end
void rtrim(std::string &s);
// trim from both ends
void trim(std::string &s);

#endif