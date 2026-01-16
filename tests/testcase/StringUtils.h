#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

class StringUtils
{
public:
    // 字符串分割
    static std::vector<std::string> split(const std::string &str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;

        while (std::getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }

    // 去除空格
    static std::string trim(const std::string &str)
    {
        size_t start = 0;
        size_t end = str.length() - 1;

        while (start <= end && std::isspace(str[start]))
            start++;
        while (end >= start && std::isspace(str[end]))
            end--;

        if (start > end)
            return "";
        return str.substr(start, end - start + 1);
    }

    // 字符串反转
    static std::string reverse(const std::string &str)
    {
        std::string result = str;
        std::reverse(result.begin(), result.end());
        return result;
    }

    // 检查是否包含子串
    static bool contains(const std::string &str, const std::string &substring)
    {
        return str.find(substring) != std::string::npos;
    }

    // 转换为大写
    static std::string toUpper(const std::string &str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    // 转换为小写
    static std::string toLower(const std::string &str)
    {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
};