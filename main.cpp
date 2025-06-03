#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "ProcessChecker/ProcessChecker.hpp"
#include "ProcessExecutor/ProcessExecutor.hpp"

#include "influxdb-cpp/influxdb.hpp"


void show_db(){
    influxdb_cpp::server_info si("192.168.1.110", 8086, "states");

    std::string resp;
    influxdb_cpp::query(resp, "select * from process_status", si);
    std::cout << resp << std::endl;
}

int main() {
    influxdb_cpp::server_info si("192.168.1.110", 8086, "states");

    std::vector<std::string> names = {
        "pythonsolar_monitor_grafana.py",
        "pythoninotify.py/home/samuel/FTP/",
        "pythonmeshtastic_recv_handler.py",
    };


    ProcessChecker checker;
    ProcessExecutor executor(checker, ProcessChecker::BucketsTypes::COMMUNICATION);


    while (true) {
        auto results = checker.checkProcesses(names);

        checker.Notify(ProcessChecker::BucketsTypes::COMMUNICATION, results);  
          
        influxdb_cpp::builder()
        .meas("process_status")
        .field("solar", results["pythonsolar_monitor_grafana.py"] )
        .field("camera", results["pythoninotify.py/home/samuel/FTP/"])
        .field("temperatur_sensor", results["pythonmeshtastic_recv_handler.py"])
        .post_http(si);

        std::this_thread::sleep_for(std::chrono::minutes(5));
        std::cout<<std::endl;
    }

    return 0;
}



