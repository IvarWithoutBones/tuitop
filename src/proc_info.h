#include <string>
#include <vector>
#include <cmath>
#include <proc/readproc.h>

namespace tuitop {
    class proc_info {
        private:
            // Rounds a number, i think? Need to properly look into this, currently just stolen from the internet.
            template<typename T>
            T round(T val, int x) {
                static const double powerOfTen[] = { 1.0, 10.0, 100.0, 1000.0 };
                return std::floor(val * powerOfTen[x]) / powerOfTen[x];
            };
        public:
            std::vector<proc_t> getRunningProcs();

            std::string getCommand(proc_t& process, int maxLength = 200);
            std::string getCmdBasename(proc_t& process);
            std::string getUser(proc_t& process);
            double getCpuPercent(proc_t& process);
    };
}
