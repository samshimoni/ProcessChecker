#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "ProcessChecker/ProcessChecker.hpp"
#include "ProcessExecutor/ProcessExecutor.hpp"
#include "influxdb-cpp/influxdb.hpp"


int main() {    
    influxdb_cpp::server_info si("localhost", 8086, "states");

    std::vector<std::string> names = {
        "pythonsolar_monitor_grafana.py",
        "pythoninotify.py/home/samuel/FTP/",
        "pythonmeshtastic_recv_handler.py",
    };


    ProcessChecker checker;
    ProcessExecutor executor(checker, ProcessChecker::BucketsTypes::COMMUNICATION);

    while (true) {
        std::vector<std::string> procesess_to_execute = {};
        auto results = checker.checkProcesses(names);
        for (const auto& [name, running] : results) {
            if (running == 0){
                procesess_to_execute.push_back(name);
            }
        }

        if (!procesess_to_execute.empty()){
            checker.Notify(ProcessChecker::BucketsTypes::COMMUNICATION, procesess_to_execute);
        }

        influxdb_cpp::builder()
        .meas("process_status")
        .field("solar", results["pythonsolar_monitor_grafana.py"] )
        .field("camera", results["pythoninotify.py/home/samuel/FTP/"])
        .field("temperatur_sensor", results["pythonmeshtastic_recv_handler.py"])
        .post_http(si);

        std::this_thread::sleep_for(std::chrono::minutes(2));
        std::cout<<std::endl;
    }

    return 0;
}



