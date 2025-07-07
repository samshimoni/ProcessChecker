#include "ProcessChecker.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

namespace fs = std::filesystem;

std::vector<std::string> splitByNull(const std::string input) {
    std::vector<std::string> result;
    size_t start = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\0') {
            result.emplace_back(input.data() + start, i - start);
            start = i + 1;
        }
    }

    if (start < input.size()) {
        result.emplace_back(input.data() + start, input.size() - start);
    }

    return result;
}

std::vector<std::vector<std::string>> ProcessChecker::checkProcesses(const std::vector<std::vector<std::string>>& processes, int& live_proc_found) {
    
    std::vector<std::vector<std::string>> result;

    for (const auto& command : processes){
        result.push_back(command);
    }
    
    for (const auto& entry : fs::directory_iterator("/proc")) {

        if (!entry.is_directory()) continue;
        std::string pid = entry.path().filename();
        if (!std::all_of(pid.begin(), pid.end(), ::isdigit)) continue;

        std::ifstream cmdFile(entry.path() / "cmdline");

        std::string procName;

        std::getline(cmdFile, procName);
                
        if (cmdFile.is_open() && "" != procName) {

            auto process_arguments_vector = splitByNull(procName);

            for (auto proc : processes){
                if (proc == process_arguments_vector){
                    std::cout << "Found " << proc.at(1) << std::endl;
                    live_proc_found++;
                    result.erase(std::remove(result.begin(), result.end(), proc), result.end());
                }
            }
        }
    }

    return result;
}


