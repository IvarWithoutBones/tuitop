#include <fmt/core.h>
#include "proc_info.h"

int main(int argc, const char* argv[]) {
    auto proc = tuitop::proc_info();

    auto proc_list = proc.getSortedProcs();

    for (proc_t i : proc_list) {
        fmt::print("TID {}: cpu = {}%, user = {}, cmd = {}\n", i.tid, proc.getCpuPercent(i), proc.getUser(i), proc.getCmdBasename(i));
    };
};
