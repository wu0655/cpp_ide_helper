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

std::unordered_set<std::string> common_main(const std::string &code_dir, const std::string &build_dir,
                                            const std::string &extname,
                                            const std::function<std::vector<std::string>(
                                                const std::string &, const std::string &,
                                                const std::string &)> &analyze_file) {
#if DEBUG
        std::cout << "code_dir = " << code_dir << std::endl;
        std::cout << "build_dir = " << build_dir << std::endl;
        std::cout << "extname = " << extname << std::endl;
#endif

    //scan directory
    std::vector<std::string> cmd_files = scan_directory(build_dir, extname);

    std::cout << cmd_files.size() << " cmd files found" << std::endl;


    //use thread pool to parse file
    std::vector<std::vector<std::string> > partial_results_cmd_files(cmd_files.size());

#if 1
    unsigned int hw_threads = std::thread::hardware_concurrency();
    unsigned int pool_size = std::min(hw_threads != 0 ? hw_threads : 4, 32u); // fallback to 4 if hw_threads is 0
    BS::thread_pool pool(pool_size);
    const BS::multi_future<void> loop_future = pool.submit_loop<unsigned int>(0, cmd_files.size(),
                                                                              [&cmd_files, &partial_results_cmd_files, &
                                                                                  code_dir, &build_dir, &analyze_file](
                                                                          const unsigned int i) {
                                                                                  partial_results_cmd_files[i] =
                                                                                          analyze_file(
                                                                                              cmd_files[i], code_dir,
                                                                                              build_dir);
                                                                              });

#if 0
    using namespace std::chrono_literals;
    while (!loop_future.wait_for(0ms)) {
        std::cout << ".";
        std::this_thread::sleep_for(1000ms);
    }
#else
    while (true) {
        if (!pool.wait_for(std::chrono::milliseconds(1000)))
            std::cout << ".";
        else {
            std::cout << std::endl;
            break;
        }
    }
#endif

#else
    for(int i=0; i<cmd_files.size();i++) {
        std::cout << "parse file=" << cmd_files[i] << std::endl;
        partial_results_cmd_files[i] = analyze_file(cmd_files[i], code_dir, build_dir);
    }
#endif

    //remove duplicate
    std::unordered_set<std::string> file_list;
    remove_duplicate_strings(partial_results_cmd_files, file_list);
    return file_list;
}

std::string get_abs_path(const std::string &in) {
    fs::path path = in;
    std::string ret;
    if (fs::exists(path)) {
        ret = absolute(path).string();
    }
    return ret;
}

std::string get_abs_path(const std::string &dir, const std::string &name) {
    const fs::path rel = name;
    fs::path path;
    if (rel.is_absolute()) {
        path = rel;
    } else {
        path = dir / rel;
    }

    std::string ret;
    if (fs::exists(path)) {
        ret = absolute(path).string();
    }
    return ret;
}
