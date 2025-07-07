#pragma once

#include <string>
#include <vector>
#include "../communication/IObserver.hpp"
#include <memory>
#include <unordered_map>


class ProcessExecutor : public IObserver {
public:
    explicit ProcessExecutor(ISubject& subject, int bucket);
    ~ProcessExecutor();
    
    void OnNotify(const std::vector<std::vector<std::string>>& message) override;
    int execute_process(const std::vector<std::string>& args);

private:
    void extract_arguments(const std::string& input, std::unique_ptr<std::vector<std::string>>& output);

    ISubject& m_subject;
    int m_bucket;
};
