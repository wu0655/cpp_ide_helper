//
// Created by wu on 25-8-3.
//

#ifndef FILE_PARSER_H
#define FILE_PARSER_H
#include <string>
#include <vector>

std::vector<std::string> analyze_kern_cmd_file(const std::string &filename, const std::string &code_dir, const std::string &build_dir);
#endif //FILE_PARSER_H
