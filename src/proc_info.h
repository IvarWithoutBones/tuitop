#include <string>
#include <vector>
#include <cmath>
#include <proc/readproc.h>
#include "common.h"

namespace tuitop {
    class ProcInfo {
        private:
            // Rounds a number, i think? Need to properly look into this, currently just stolen from the internet.
            template<typename T>
            T round(T val, int x) {
                static const double powerOfTen[] = { 1.0, 10.0, 100.0, 1000.0 };
                return std::floor(val * powerOfTen[x]) / powerOfTen[x];
            };

            std::string getCommand(proc_t& process);
            std::string getCmdBasename(proc_t& process);
            std::string getUser(proc_t& process);
            std::string getCpuPercent(proc_t& process);
            std::string removeNixStorePath(std::string);
        public:
            const std::vector<tuitop::proc> getProcs();
    };
}
