#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <linux/limits.h> // for PATH_MAX
#include <pwd.h> // For getpwuid
#include <fmt/core.h>
#include <proc/readproc.h>
#include <proc/sysinfo.h> // For uptime, Hertz

#include "proc_info.h"

namespace tuitop {
    std::vector<proc_t> proc_info::getRunningProcs() {
        std::vector<proc_t> readableProcs;

        proc_t process_info;
        memset(&process_info, 0, sizeof(process_info));

        PROCTAB* procData = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
        while (readproc(procData, &process_info) != NULL) {
            readableProcs.push_back(process_info);
        };

        closeproc(procData);

        return readableProcs;
    };

    std::vector<proc_t> proc_info::getSortedProcs(bool const sort_by_cpu) {
        std::vector<proc_t> proc_list = getRunningProcs();

        if (sort_by_cpu) {
            std::sort(proc_list.begin(), proc_list.end(), [this, sort_by_cpu](proc_t lhs, proc_t rhs) {
                return proc_info::getCpuPercent(lhs) > proc_info::getCpuPercent(rhs);
            });
        }

        return proc_list;
    };

    std::string proc_info::getCommand(proc_t& process, int const maxLength) {
        std::string path = fmt::format("/proc/{}/cmdline", process.tid);
        std::string buf = std::string(PATH_MAX, '\0');
        std::string result;

        auto stream = std::ifstream(path.data());
        stream.exceptions(std::ios_base::badbit);

        while (stream.read(& buf[0], PATH_MAX)) {
            result.append(buf, 0, stream.gcount());
        };

        result.append(buf, 0, stream.gcount());
        stream.close();

        if (stream.gcount() > maxLength)
            result.resize(maxLength);

        return result;
    };

    std::string proc_info::getCmdBasename(proc_t& process) {
        return static_cast<std::string>(process.cmd);
    };

    std::string proc_info::getUser(proc_t& process) {
        passwd *user = getpwuid(static_cast<uid_t>(process.euid));

        if (user != NULL) {
            auto user_str = std::string(user->pw_name);
            return user_str;
        } else {
            return std::to_string(process.euid);
        };
    };

    double proc_info::getCpuPercent(proc_t& process) {
        // I have no idea what this is doing, this has been stolen from the internet.
        time_t total_time = process.utime + process.stime;
        time_t sec_since_boot = uptime(NULL, NULL);
        time_t seconds = sec_since_boot - process.start_time / Hertz;

        double pcpu = static_cast<double>((total_time * 1000ULL / Hertz) / (seconds*10.0));
        double result = round(pcpu, 1);

        // TODO: NaN and inf are reported sometimes?
        return result;
    };
}

