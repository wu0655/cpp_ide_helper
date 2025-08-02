#pragma once

#include <string>
#include <vector>
#include <regex>
#include <algorithm>

namespace string_utils {

// 去除前后空白
inline std::string strip(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    const auto end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// 按单个字符分割
inline std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : s) {
        if (c == delimiter) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(token);  // 最后一个
    return tokens;
}

// 使用正则表达式分割（如多个空格）
inline std::vector<std::string> split_regex(const std::string& s, const std::string& pattern) {
    std::vector<std::string> tokens;
    std::regex re(pattern);
    std::sregex_token_iterator it(s.begin(), s.end(), re, -1);
    std::sregex_token_iterator end;
    for (; it != end; ++it) {
        std::string token = strip(it->str());
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

// 是否以 prefix 开头
inline bool starts_with(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

// 是否以 suffix 结尾
inline bool ends_with(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

}  // namespace text_utils
