#include <iostream>
#include <filesystem>
#include <fstream>
#include <future>
#include <vector>
#include <mutex>
#include <thread>
#include <unordered_set>

#include "BS_thread_pool.hpp"
#include "string_utils.h"
#include "common.h"


#define MAX_THEAD_NUM 32

namespace fs = std::filesystem;


std::vector<std::string> scan_directory(const std::string &directory_path, const std::string &extname) {
    std::vector<std::string> result;

    for (const auto &entry: fs::recursive_directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
            const std::string filename = entry.path().filename().string();

            if (filename.size() >= 6 && string_utils::ends_with(filename, extname)) {
                result.push_back(entry.path().string());
            }
        }
    }

    return result;
}

void remove_duplicate_strings(std::vector<std::vector<std::string> > &in, std::unordered_set<std::string> &seen) {
    for (auto &inner_vec: in) {
        for (const auto &s: inner_vec) {
            seen.insert(s);
        }
    }
}

std::string get_canonical_path(const std::string &in) {
    fs::path p(in);
    if (!fs::exists(p)) {
        return "";
    }
    try {
        return fs::canonical(p).string();
    } catch (const fs::filesystem_error &) {
        return ""; // 避免异常中断
    }
}

std::string get_canonical_path(const std::string &dir, const std::string &name) {
    const fs::path rel = name;
    fs::path path;
    if (rel.is_absolute()) {
        path = rel;
    } else {
        path = dir / rel;
    }

    try {
        return fs::canonical(path).string();
    } catch (const fs::filesystem_error &) {
        return ""; // 避免异常中断
    }
}

std::unordered_set<std::string> multi_thread_analyze(const std::vector<std::string> &cmd_files, const std::string &base_dir,
                                                     const std::function<std::vector<std::string>(
                                                         const std::string &, const std::string &)> &analyze_file) {
    //use thread pool to parse file
    std::vector<std::vector<std::string> > partial_results_cmd_files(cmd_files.size());

#if 1//BS_THREAD_POOL_NUM
    unsigned int hw_threads = std::thread::hardware_concurrency();
    unsigned int pool_size = std::min(hw_threads != 0 ? hw_threads : 4, BS_THREAD_POOL_NUM); // fallback to 4 if hw_threads is 0
    BS::thread_pool pool(pool_size);
    const BS::multi_future<void> loop_future = pool.submit_loop<unsigned int>(0, cmd_files.size(),
                                                                              [&cmd_files, &partial_results_cmd_files, &
                                                                                  base_dir, &analyze_file](
                                                                          const unsigned int i) {
                                                                                  partial_results_cmd_files[i] =
                                                                                          analyze_file(
                                                                                              cmd_files[i], base_dir);
                                                                              });

    while (true) {
        if (!pool.wait_for(std::chrono::milliseconds(100)))
            std::cout << ".";
        else {
            std::cout << std::endl;
            break;
        }
    }

#else
    for(int i=0; i<cmd_files.size();i++) {
        //std::cout << "parse file=" << cmd_files[i] << std::endl;
        partial_results_cmd_files[i] = analyze_file(cmd_files[i], base_dir);
    }
#endif

    //remove duplicate
    std::unordered_set<std::string> file_list;
    remove_duplicate_strings(partial_results_cmd_files, file_list);
    return file_list;
}

std::unordered_set<std::string> single_thread_analyze(const std::vector<std::string> &cmd_files, const std::string &base_dir,
                                                     const std::function<std::vector<std::string>(
                                                         const std::string &, const std::string &)> &analyze_file) {
    //use thread pool to parse file
    std::vector<std::vector<std::string> > partial_results_cmd_files(cmd_files.size());

    for(int i=0; i<cmd_files.size();i++) {
        std::cout << "parse file=" << cmd_files[i] << std::endl;
        partial_results_cmd_files[i] = analyze_file(cmd_files[i], base_dir);
    }

    //remove duplicate
    std::unordered_set<std::string> file_list;
    remove_duplicate_strings(partial_results_cmd_files, file_list);
    return file_list;
}


std::string find_build_path(const std::string &in) {
    std::string ret;
    fs::path path = in;
    if (fs::exists(path)) {
        while (true) {
            fs::path p = path / ".config";
            if (fs::exists(p) && fs::is_regular_file(p)) {
                ret = absolute(path).string();
                break;
            }
            if (path == path.root_path()) {
                break;  // 已到达根目录
            }
            path = path.parent_path();
        }
    }
    return ret;
}