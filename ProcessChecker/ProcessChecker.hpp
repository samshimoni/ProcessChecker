#ifndef PROCESSCHECKER_H
#define PROCESSCHECKER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../communication/ISubject.hpp"

class ProcessChecker : public ISubject {

public:
    std::vector<std::vector<std::string>> checkProcesses(const std::vector<std::vector<std::string>>& processes, int& live_pro_found);
    enum BucketsTypes{COMMUNICATION};
};

#endif // PROCESSCHECKER_H
