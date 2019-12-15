#ifndef SWITCH_CONTROLLER_HPP
#define SWITCH_CONTROLLER_HPP

#include <string>

struct switch_controller {
    int pid;
    int vid;
    std::string device_name;
    std::string mac_address;
    bool is_rebooted;
};

#endif // SWITCH_CONTROLLER_HPP
