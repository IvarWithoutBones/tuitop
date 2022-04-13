#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <fmt/core.h>
#include <proc/readproc.h>
#include <proc/sysinfo.h> // For uptime, Hertz
#include "proc_info.h"

namespace tuitop {
    const std::vector<tuitop::proc> ProcInfo::getProcs() {
        PROCTAB* procData = openproc(PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLUSR);
        std::vector<tuitop::proc> readableProcs;
        proc_t process_info{};

        while (readproc(procData, &process_info) != NULL) {
            tuitop::proc i;

            i.command = getCommand(process_info);
            i.pid = std::to_string(process_info.tid);
            i.user = process_info.suser;
            i.cmdBasename = process_info.cmd;
            i.cpuPercent = getCpuPercent(process_info);

            readableProcs.push_back(i);
        };

        closeproc(procData);

        // sort by cpu usage
        std::sort(readableProcs.begin(), readableProcs.end(), [](tuitop::proc lhs, tuitop::proc rhs) {
            return std::stof(lhs.cpuPercent) > std::stof(rhs.cpuPercent);
        });

        return readableProcs;
    };

    std::string ProcInfo::getCommand(proc_t& process) {
        // TODO: should probably use process.cmdline instead
        std::string path = fmt::format("/proc/{}/cmdline", process.tid);
        std::string result;
        bool isFirstLine = true;

        auto stream = std::ifstream(path.data());
        stream.exceptions(std::ios_base::badbit);

        for (std::string line; std::getline(stream, line, '\0'); ) {
            if (isFirstLine)
                isFirstLine = false;
            else
                result += " ";

            result += line;
        };

        stream.close();

        return tuitop::removeNixStorePath(result);
    };

    // TODO: NaN and inf are reported sometimes?
    std::string ProcInfo::getCpuPercent(proc_t& process) {
        // I have no idea what this is doing, it has been stolen
        time_t total_time = process.utime + process.stime;
        time_t sec_since_boot = uptime(NULL, NULL);
        time_t seconds = sec_since_boot - process.start_time / Hertz;

        double pcpu = static_cast<double>((total_time * 1000ULL / Hertz) / (seconds*10.0));
        double result = round(pcpu, 1);

        auto resultStr = std::to_string(result);
        auto dotPos = resultStr.find(".");

        if (result < 10) {
            return resultStr.substr(0, dotPos+2);
        } else {
            return resultStr.substr(0, dotPos);
        };
    };
}
