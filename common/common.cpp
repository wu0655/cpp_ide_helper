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

void print_progress_bar(unsigned int completed, size_t total) {
#if 1
    static int x = 0;
    const int bar_width = 50;
    auto progress = static_cast<float>(completed) / total;

    std::cout << "\r[";
    int pos = static_cast<int>(bar_width * progress);
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(progress * 100.0) << " %";
    std::cout.flush();
    //std::cout << x++;
#else
    std::cout << '.' << std::flush;
#endif
}

std::unordered_set<std::string> multi_thread_analyze(const std::vector<std::string> &cmd_files, const std::string &base_dir,
                                                     const std::function<std::vector<std::string>(
                                                         const std::string &, const std::string &, std::atomic<unsigned int>&)> &analyze_file) {







    //use thread pool to parse file
    std::vector<std::vector<std::string> > partial_results_cmd_files(cmd_files.size());

#if 1//BS_THREAD_POOL_NUM
    unsigned int hw_threads = std::thread::hardware_concurrency();
    unsigned int pool_size = std::min(hw_threads != 0 ? hw_threads : 4, BS_THREAD_POOL_NUM); // fallback to 4 if hw_threads is 0
    BS::thread_pool pool(pool_size);
#if 1
    std::atomic<bool> done{false};
    std::atomic<unsigned int> completed_tasks{0};
    const size_t total_tasks = cmd_files.size();
    std::thread progress_thread([&]() {
        while (!done) {
            print_progress_bar(completed_tasks, total_tasks);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        // Ensure full bar printed at the end
        print_progress_bar(total_tasks, total_tasks);
        std::cout << "\n";
    });
#endif
    const BS::multi_future<void> loop_future = pool.submit_loop<unsigned int>(0, cmd_files.size(),
                                                                              [&cmd_files, &partial_results_cmd_files, &
                                                                                  base_dir, &analyze_file, &completed_tasks](
                                                                          const unsigned int i) {
                                                                                  partial_results_cmd_files[i] =
                                                                                          analyze_file(cmd_files[i], base_dir, std::ref(completed_tasks));
                                                                              });



    pool.wait();
    done = true;
    progress_thread.join();

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