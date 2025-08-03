//
// Created by wu on 25-8-3.
//

#ifndef COMMON_H
#define COMMON_H
#include <functional>
#include <string>
#include <unordered_set>

#define DEBUG 0
#define BS_THREAD_POOL_ENABLE 1

std::unordered_set<std::string> common_main(const std::string &code_dir, const std::string &build_dir, const std::string &extname,
                                            const std::function<std::vector<std::string>(const std::string&, const std::string&, const std::string&)>& analyze_file);
std::string get_abs_path(const std::string &in);
std::string get_abs_path(const std::string &dir, const std::string &name);
#endif //COMMON_H
