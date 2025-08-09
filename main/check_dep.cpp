#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>

#include "common.h"
#include "string_utils.h"

void print_usage(const char *program_name) {
    std::cerr << "Usage: " << program_name
            << " --in <in_file> --code <dir> --build <dir> --out <out_file>\n";
}

int main(int argc, char* argv[]) {
    std::unordered_map<std::string, std::string> args;

    for (int i = 1; i < argc; i += 2) {
        std::string key = argv[i];
        std::string value = argv[i + 1];
        args[key] = value;
    }

    // Required arguments
    const std::string required_keys[] = {"--code", "--build", "--in", "--out"};
    for (const auto &key: required_keys) {
        if (args.find(key) == args.end()) {
            std::cerr << "Missing argument: " << key << "\n";
            print_usage(argv[0]);
            return 1;
        }
    }

    const std::string &code_dir = get_canonical_path(args["--code"]);
    const std::string &build_dir = get_canonical_path(args["--build"]);
    const std::string &in_file = args["--in"];
    const std::string &out_file = args["--out"];

    if (code_dir.empty() || build_dir.empty()) {
        print_usage(argv[0]);
        return 1;
    }


    std::ifstream infile(in_file);
    if (!infile) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::unordered_set<std::string> seen;
    std::string line;
    std::unordered_set<std::string> deps;
    std::unordered_set<std::string> other;

    while (std::getline(infile, line)) {
        // Check if this line has been seen before
        if (seen.find(line) == seen.end()) {
            seen.insert(line);
            if (string_utils::starts_with(line, code_dir) || string_utils::starts_with(line, build_dir)) {
                deps.insert(line);
            } else {
                other.insert(line);
            }
        }
    }

    std::ofstream out(out_file);
    if (!out) {
        std::cerr << "Failed to open file: " << out_file << std::endl;
        return -1;
    }

    std::ofstream out_other(out_file + ".other");
    if (!out_other) {
        std::cerr << "Failed to open file: " << out_file << std::endl;
        return -1;
    }

    //save file
    for (const auto &str: deps) {
        out << str << '\n';
    }

    //save file
    for (const auto &str: other) {
        out_other << str << '\n';
    }

    return 0;
}
