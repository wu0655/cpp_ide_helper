//
// Created by wu on 25-8-3.
//

#ifndef COMMON_H
#define COMMON_H
#include <functional>
#include <string>
#include <unordered_set>

#define DEBUG 0
#define BS_THREAD_POOL_NUM (64u)

#include <atomic>
std::vector<std::string> scan_directory(const std::string &directory_path, const std::string &extname);
std::string get_canonical_path(const std::string &in);
std::string get_canonical_path(const std::string &dir, const std::string &name);
void remove_duplicate_strings(std::vector<std::vector<std::string> > &in, std::unordered_set<std::string> &seen);
std::unordered_set<std::string> multi_thread_analyze(const std::vector<std::string> &cmd_files, const std::string &base_dir,
                                                     const std::function<std::vector<std::string>(
                                                         const std::string &,
                                                         const std::string &, std::atomic<unsigned int>&)> &analyze_file);
std::unordered_set<std::string> single_thread_analyze(const std::vector<std::string> &cmd_files, const std::string &code_dir,
                                                     const std::function<std::vector<std::string>(
                                                         const std::string &,
                                                         const std::string &)> &analyze_file);
std::string find_build_path(const std::string &in);
#endif //COMMON_H
