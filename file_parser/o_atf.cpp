#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>


#include "../common/common.h"
#include "../common/string_utils.h"


#define MAX_THEAD_NUM 32

namespace fs = std::filesystem;


std::vector<std::string> analyze_atf_o_file(const std::string &filename, const std::string &base_dir) {
    std::ifstream infile(filename);
    std::vector<std::string> result;
    std::vector<std::string> deps_vec;

    if (!infile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return result;
    }
    //get out directory when build
    //std::string build_out = get_build_path(build_dir);

    std::string line;
    std::string buffer;


    while (std::getline(infile, line)) {
        if (line.back() == ':' || line.back() == '\\') {
            line.pop_back(); // 移除末尾的 '\'
        }
        line.append("  ");
        buffer.append(line);
    }

    //std::cout << "buffer=" << buffer << std::endl;

    size_t pos = buffer.find(":");
    if (pos != std::string::npos) {
        std::string values_part = buffer.substr(pos + 1);
        deps_vec = string_utils::split_regex(values_part, "\\s{1,}");
    }

    //std::cout << "deps_vec.size()=" << deps_vec.size() << std::endl;

    for (auto &entry: deps_vec) {
        // save absolute path if exist
        if (fs::path p = entry; p.is_absolute() && fs::exists(p)) {
            result.emplace_back(entry);
        }

        // convert relative path to absolute path
        std::string p = get_canonical_path(base_dir, entry);
        if (!p.empty())
            result.emplace_back(p);
        else {
            std::cout << "file=" << filename << " entry=" << entry << std::endl;
        }
    }

    return result;
}
