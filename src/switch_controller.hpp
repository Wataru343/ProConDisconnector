#ifndef SWITCH_CONTROLLER_HPP
#define SWITCH_CONTROLLER_HPP

#include <chrono>
#include <string>

struct switch_controller {
    int pid;
    int vid;
    std::string device_name;
    std::string mac_address;
    std::chrono::time_point<std::chrono::steady_clock> atached_time;
    bool is_rebooted;
};

#endif // SWITCH_CONTROLLER_HPP
