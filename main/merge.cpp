#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " input_file1 [input_file2 ...] [-o output_file]" << std::endl;
        return 1;
    }

    std::vector<std::string> input_files;
    std::string output_file;
    bool output_to_file = false;

    // 解析参数
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o") {
            if (i + 1 < argc) {
                output_file = argv[i + 1];
                output_to_file = true;
                i++; // 跳过文件名参数
            } else {
                std::cerr << "Error: -o requires an argument." << std::endl;
                return 1;
            }
        } else {
            input_files.push_back(arg);
        }
    }

    if (input_files.empty()) {
        std::cerr << "No input files specified." << std::endl;
        return 1;
    }

    std::unordered_set<std::string> seen;
    std::vector<std::string> unique_lines;

    for (const auto& filename : input_files) {
        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Warning: cannot open file " << filename << ", skipped." << std::endl;
            continue;
        }
        std::string line;
        while (std::getline(infile, line)) {
            if (seen.insert(line).second) {  // 新行才加入
                unique_lines.push_back(line);
            }
        }
    }

    if (output_to_file) {
        std::ofstream outfile(output_file);
        if (!outfile) {
            std::cerr << "Error: cannot open output file " << output_file << std::endl;
            return 1;
        }
        for (const auto& line : unique_lines) {
            outfile << line << '\n';
        }
    } else {
        // 输出到标准输出
        for (const auto& line : unique_lines) {
            std::cout << line << '\n';
        }
    }

    std::cout << std::endl;
    std::cout << "RESULT: total " << unique_lines.size() << " files found." << std::endl;
    return 0;
}
