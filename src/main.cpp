#include <algorithm>
#include <fmt/core.h>
#include "proc_info.h"

int main(int argc, const char* argv[]) {
    auto proc = tuitop::proc_info();

    std::vector<proc_t> current = proc.getRunningProcs();
    std::sort(current.begin(), current.end(), [&proc](proc_t lhs, proc_t rhs) {
        return proc.getCpuPercent(lhs) < proc.getCpuPercent(rhs);
    });

    for (proc_t i : current) {
        fmt::print("TID {}: cpu = {}%, user = {}, cmd = {}\n", i.tid, proc.getCpuPercent(i), proc.getUser(i), proc.getCmdBasename(i));
    };
};
