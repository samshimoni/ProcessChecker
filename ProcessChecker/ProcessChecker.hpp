#ifndef PROCESSCHECKER_H
#define PROCESSCHECKER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../communication/ISubject.hpp"

class ProcessChecker : public ISubject {

public:
    std::unordered_map<std::string, int> checkProcesses(const std::vector<std::string>& names);

    enum BucketsTypes{COMMUNICATION};
private:
};

#endif // PROCESSCHECKER_H
