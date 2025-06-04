#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <limits.h>

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
        "pythonalarm_server.py"
    };


    ProcessChecker checker;
    ProcessExecutor executor(checker, ProcessChecker::BucketsTypes::COMMUNICATION);

    int last_time_running_process_count =  INT_MAX;

    while (true) {
        int running_process_count = 0;
        auto results = checker.checkProcesses(names, running_process_count);
        
        if (running_process_count != last_time_running_process_count){
            checker.Notify(ProcessChecker::BucketsTypes::COMMUNICATION, results);  
            last_time_running_process_count = running_process_count;

            influxdb_cpp::builder()
            .meas("process_status")
            .field("solar", results["pythonsolar_monitor_grafana.py"] )
            .field("camera", results["pythoninotify.py/home/samuel/FTP/"])
            .field("temperatur_sensor", results["pythonmeshtastic_recv_handler.py"])
            .field("alarm", results["pythonalarm_server.py"])
            .post_http(si);
        }
          
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }

    return 0;
}



