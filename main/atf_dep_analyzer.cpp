#include <atomic>
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


namespace fs = std::filesystem;


void print_usage(const char *program_name) {
    std::cerr << "Usage: " << program_name
            << " --in <input .cmd list> --code <dir> --out <out_file>\n";
}



int main(int argc, char *argv[]) {
    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i < argc; i += 2) {
        std::string key = argv[i];
        std::string value = argv[i + 1];
        args[key] = value;
    }

    // Required arguments
    const std::string required_keys[] = {"--in", "--code", "--out"};
    for (const auto &key: required_keys) {
        if (args.find(key) == args.end()) {
            std::cerr << "Missing argument: " << key << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    const std::string &code_dir = get_canonical_path(args["--code"]);
    const std::string &in_file = args["--in"];
    const std::string &out_file = args["--out"];

    if (code_dir.empty()) {
        print_usage(argv[0]);
        return 1;
    }

    if (DEBUG) {
        std::cout << "in_file = " << in_file << std::endl;
        std::cout << "code_dir = " << code_dir << std::endl;
        std::cout << "out_file = " << out_file << std::endl;
    }

    //read
    std::ifstream infile(in_file);
    if (!infile) {
        std::cerr << "Failed to open file\n";
        return -1;
    }
    std::unordered_set<std::string> seen;
    std::string line;
    while (std::getline(infile, line)) {
        // Check if this line has been seen before
        if (seen.find(line) == seen.end()) {
            seen.insert(line);
        }
    }
    std::vector<std::string> cmd_files(seen.begin(), seen.end());

    //analyze
    std::function chosen_analyze = analyze_atf_o_file;
    std::string base_dir = find_build_path(code_dir);
    std::cout << "base_dir=" << base_dir  << std::endl;
    std::unordered_set<std::string> file_list = multi_thread_analyze(cmd_files, base_dir, chosen_analyze);

    //save file
    std::ofstream out(out_file);
    if (!out) {
        std::cerr << "Failed to open file: " << out_file << std::endl;
        return -1;
    }
    for (const auto &str: file_list) {
        out << str << '\n';
    }

    std::cout << file_list.size() << " source files found"  << std::endl;
    return 0;
}