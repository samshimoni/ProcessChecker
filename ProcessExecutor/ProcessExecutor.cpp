#include "ProcessExecutor.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <unordered_map>
#include <cstring> 

ProcessExecutor::ProcessExecutor(ISubject &subject, int bucket) : m_subject(subject), m_bucket(bucket)
{
    m_subject.AddObserver(m_bucket, this);
}

ProcessExecutor::~ProcessExecutor()
{
    m_subject.RemoveObserver(m_bucket, this);
}

void ProcessExecutor::OnNotify(const std::vector<std::vector<std::string>>& message)
{
    for (const auto& command : message){
        this->execute_process(command);
    }
}

int ProcessExecutor::execute_process(const std::vector<std::string> &args)
{
    if (args.empty()) {
        std::cerr << "No command provided." << std::endl;
        return -1;
    }

    std::vector<char*> c_args;
    for (const auto& arg : args) {
        c_args.push_back(const_cast<char*>(arg.c_str()));
    }
    c_args.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // In child process
        setsid();  // Detach from parent terminal/session
        execvp(c_args[0], c_args.data());

        // Only reached if execvp fails
        perror("execvp");
        _exit(1);
    }

    // Parent returns immediately with child PID (or 0/success indicator)
    std::cout << "Process started with PID " << pid << std::endl;
    return 0;
}