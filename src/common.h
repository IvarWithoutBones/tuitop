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

    inline std::string removeNixStorePath(std::string string) {
        std::string nixStorePath = "/nix/store/";
        int storeIndex = string.find(nixStorePath);

        while (storeIndex != std::string::npos) {
            // Include the first six characters of the hash
            int hashStart = storeIndex + nixStorePath.length() + 6;
            int hashEnd = string.find("-", storeIndex + 1);

            if (hashEnd != std::string::npos) {
                string.erase(string.begin() + hashStart, string.begin() + hashEnd);
            };

            storeIndex = string.find(nixStorePath, storeIndex + 1);
        };

        return string;
    };
}
