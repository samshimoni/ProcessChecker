#include "ProcessExecutor.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <unordered_map>
ProcessExecutor::ProcessExecutor(ISubject &subject, int bucket) : m_subject(subject), m_bucket(bucket)
{
    m_subject.AddObserver(m_bucket, this);
}

ProcessExecutor::~ProcessExecutor()
{
    m_subject.RemoveObserver(m_bucket, this);
}

void ProcessExecutor::OnNotify(const std::unordered_map<std::string, int>& message)
{
    for (const auto& [process, running] : message) {
        if (0 == running){
            auto process_args = std::make_unique<std::vector<std::string>>();
            this->extract_arguments(process, process_args);

            if (!process_args->empty()) {
                int result = execute_process(*process_args);
                if (result != 0) {
                    std::cerr << "Failed to execute process: " << process << std::endl;
                }
            }

            else {
                std::cerr << "No valid arguments extracted from: " << process << std::endl;
            }
        }
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

void ProcessExecutor::extract_arguments(const std::string &input, std::unique_ptr<std::vector<std::string>> &output)
 {
    const std::string prefix = "python";

    if (input.rfind(prefix, 0) == 0) { 
        output->push_back(prefix);

        size_t pos = input.substr(prefix.size()).find('/');
        if (pos != std::string::npos) {
            std::string before = input.substr(prefix.size(), pos);
            output->push_back(before);
            output->push_back(input.substr(prefix.size() + before.size()));
        } 
        
        else {
            output->push_back(input.substr(prefix.size()));
        }
    }
}
