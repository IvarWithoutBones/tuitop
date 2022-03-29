#include <string>

#pragma once

namespace tuitop {
    struct proc {
        std::string pid;
        std::string user;
        std::string command;
        std::string cmdBasename;
        std::string cpuPercent;
    };
}
