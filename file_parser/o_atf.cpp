#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>


#include "../common/common.h"
#include "../common/string_utils.h"


#define MAX_THEAD_NUM 32

namespace fs = std::filesystem;

static std::string get_build_path(const std::string &in) {
    std::string ret;
    fs::path path = in;
    if (fs::exists(path)) {
        while (true) {
            fs::path p = path / ".config";
            if (fs::exists(p) && fs::is_regular_file(p)) {
                ret = absolute(path).string();
                break;
            }
            path = path.parent_path();
        }
    }
    return ret;
}

std::vector<std::string> analyze_atf_o_file(const std::string &filename, const std::string &code_dir,
                                            const std::string &build_dir) {
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
        std::string p = get_abs_path(code_dir, entry);
        if (!p.empty())
            result.emplace_back(p);
        else {
            std::cout << "file=" << filename << " entry=" << entry << std::endl;
        }
    }

    return result;
}
