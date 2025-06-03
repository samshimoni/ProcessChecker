#include "ProcessChecker.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

std::unordered_map<std::string, int> ProcessChecker::checkProcesses(const std::vector<std::string>& names) {
    std::unordered_map<std::string, int> result;
    for (const auto& name : names) {
        result[name] = 0;
    }

    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        std::string pid = entry.path().filename();
        if (!std::all_of(pid.begin(), pid.end(), ::isdigit)) continue;

        std::ifstream cmdFile(entry.path() / "cmdline");

        std::string procName;
        std::getline(cmdFile, procName);
        
        procName.erase(std::remove(procName.begin(), procName.end(), '\0'), procName.end());

        if (cmdFile.is_open() && "" != procName) {
            for (const auto& name : names) {
                if (procName == name){
                    result[name] = 1;
                    /*TODO remove the last item no need to search for him anymore*/
                }
            }
        }
    }

    return result;
}


