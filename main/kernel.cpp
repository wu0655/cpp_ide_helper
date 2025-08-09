#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <mutex>
#include <map>
#include <thread>
#include <unordered_set>


#include "common.h"
#include "../common/string_utils.h"
#include "../file_parser/file_parser.h"


#define MAX_THEAD_NUM 32

namespace fs = std::filesystem;


void print_usage(const char *program_name) {
    std::cerr << "Usage: " << program_name
            << " --code <dir> --build <dir> --extname <name> --out <out_file>\n";
}

int main(int argc, char *argv[]) {

    std::unordered_map<std::string, std::string> args;

    for (int i = 1; i < argc; i += 2) {
        std::string key = argv[i];
        std::string value = argv[i + 1];
        args[key] = value;
    }

    // Required arguments
    const std::string required_keys[] = {"--code", "--build", "--extname", "--out"};
    for (const auto &key: required_keys) {
        if (args.find(key) == args.end()) {
            std::cerr << "Missing argument: " << key << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    const std::string &code_dir = get_canonical_path(args["--code"]);
    const std::string &build_dir = get_canonical_path(args["--build"]);
    const std::string &extname = args["--extname"];
    const std::string &out_file = args["--out"];

    if (code_dir.empty() || build_dir.empty()) {
        print_usage(argv[0]);
        return 1;
    }

    if (DEBUG) {
        std::cout << "code_dir = " << code_dir << std::endl;
        std::cout << "build_dir = " << build_dir << std::endl;
        std::cout << "extname = " << extname << std::endl;
        std::cout << "out_file = " << out_file << std::endl;
    }

    std::ofstream out(out_file);
    if (!out) {
        std::cerr << "Failed to open file: " << out_file << std::endl;
        return -1;
    }

    //scan
    std::vector<std::string> cmd_files = scan_directory(build_dir, extname);
    std::cout << cmd_files.size() << " cmd files found" << std::endl;

    //analyze
    std::function chosen_analyze = analyze_kern_cmd_file;
    std::string base_dir = find_build_path(build_dir);
    std::unordered_set<std::string> file_list = multi_thread_analyze(cmd_files, base_dir, chosen_analyze);

    //save file
    for (const auto &str: file_list) {
        out << str << '\n';
    }

    std::cout << file_list.size() << " source files found"  << std::endl;
    return 0;
}