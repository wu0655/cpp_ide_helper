#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>


#include "../common/common.h"
#include "../common/string_utils.h"

#define DEBUG 0

#define MAX_THEAD_NUM 32

namespace fs = std::filesystem;

std::vector<std::string> analyze_kern_cmd_file(const std::string &filename, const std::string &code_dir, const std::string &build_dir) {
    std::ifstream infile(filename);
    std::vector<std::string> result;
    std::vector<std::string> deps_vec;

    if (!infile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }

    std::string line;
    std::string buffer;

    int source_found = 0;
    int deps_found = 0;
    while (std::getline(infile, line)) {
        // 移除 Windows 换行
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (!line.empty() && line.back() == '\\') {
            line.pop_back(); // 移除末尾的 '\'
            buffer.append(line);
        } else {
            buffer.append(line);

            // 找到以 deps_arch 开头的行
            if (string_utils::starts_with(buffer, "source_")) {
                source_found ++;
                size_t pos = buffer.find(":=");
                if (pos != std::string::npos) {
                    std::string values_part = buffer.substr(pos + 2);
                    values_part = string_utils::strip(values_part);
                    std::string path = get_abs_path(build_dir, values_part);
                    if (!path.empty() ) {
                        result.emplace_back(path);
                    } else {
                        std::cout << "values_part = " << values_part << std::endl;
                    }
                }
            }

            // 找到以 deps_arch 开头的行
            if (string_utils::starts_with(buffer, "deps_")) {
                deps_found ++;
                size_t pos = buffer.find(":=");
                if (pos != std::string::npos) {
                    std::string values_part = buffer.substr(pos + 2);
                    values_part = string_utils::strip(values_part);
                    deps_vec = string_utils::split_regex(values_part, "\\s{2,}");
                }
            }

            buffer.clear(); // 清空合并缓冲
        }
    }

#if DEBUG
        if ((source_found != 1) && (deps_found != 1)) {
            std::cout << "module file found: " << source_found << ":" << deps_found << ":" << filename << std::endl;
        }
#endif

    for (auto &entry: deps_vec) {
        if (fs::path p = entry; p.is_absolute() && fs::exists(p)) {
            result.emplace_back(entry);
        }

        if (!string_utils::starts_with(entry, "$(wildcard")) {
            std::string p = get_abs_path(build_dir, entry);
            if (!p.empty())
                result.emplace_back(p);
            else {
                std::cout << "file=" << filename << " entry=" << entry << std::endl;
            }
        }
    }
    std::cout << ".";
    return result;
}