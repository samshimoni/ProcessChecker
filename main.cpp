#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <limits.h>

#include "ProcessChecker/ProcessChecker.hpp"
#include "ProcessExecutor/ProcessExecutor.hpp"

#include "influxdb-cpp/influxdb.hpp"
#include <algorithm>


void show_db(){
    influxdb_cpp::server_info si("192.168.1.110", 8086, "states");

    std::string resp;
    influxdb_cpp::query(resp, "select * from process_status", si);
    std::cout << resp << std::endl;
}

bool process_present(const std::vector<std::vector<std::string>>& container,
                      const std::vector<std::string>& target)
{
    return std::any_of(
        container.begin(),
        container.end(),
        [&target](const std::vector<std::string>& inner) {
            return inner == target;
        }
    );
}

int main() {
    influxdb_cpp::server_info si("192.168.1.110", 8086, "states");

    std::vector<std::string> solar = { "python", "solar_monitor_grafana.py" };
    std::vector<std::string> mesh = { "python", "meshtastic_recv_handler.py" };
    std::vector<std::string> inotify = { "python", "inotify.py", "/home/samuel/FTP/" };
    std::vector<std::string> alarm = { "python",  "alarm_server.py" };

    std::vector<std::vector<std::string>> processess = {solar, mesh, inotify, alarm};

    ProcessChecker checker;
    ProcessExecutor executor(checker, ProcessChecker::BucketsTypes::COMMUNICATION);
    int last_time_process_alive = INT_MAX;

    while (true) {

        int current_proc_alive = 0;

        auto processess_not_found = checker.checkProcesses(processess, current_proc_alive);
    
        if (processess_not_found.size() > 0){
            checker.Notify(ProcessChecker::BucketsTypes::COMMUNICATION, processess_not_found);  
        }

        
        if (last_time_process_alive != current_proc_alive)
        {
            last_time_process_alive = current_proc_alive;
            std::cout << "updating influx_db" << std::endl;
            
            influxdb_cpp::builder()
            .meas("process_status")
            .field("solar", process_present(processess_not_found, { "python", "solar_monitor_grafana.py" }) ? 0 : 1)
            .field("camera", process_present(processess_not_found, { "python", "inotify.py", "/home/samuel/FTP/" }) ? 0 : 1)
            .field("temperatur_sensor", process_present(processess_not_found, { "python", "meshtastic_recv_handler.py" }) ? 0 : 1)
            .field("alarm", process_present(processess_not_found, { "python",  "alarm_server.py" }) ? 0 : 1)
            .post_http(si);
        }
          
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }

    return 0;
}



