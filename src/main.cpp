#include <fmt/core.h>
#include "proc_info.h"
#include "ui.h"

int main(int argc, const char* argv[]) {
    auto proc_info = tuitop::proc_info();
    auto user_interface = tuitop::UserInterface();

    auto proc_list = proc_info.getSortedProcs();

    for (proc_t &i : proc_list) {
        auto tid = std::to_string(i.tid);
        auto user = proc_info.getUser(i);
        auto cpuPercent = fmt::format("{}%", proc_info.getCpuPercent(i));
        auto cmdBasename = proc_info.getCommand(i);
        user_interface.addProcess(tid, user, cpuPercent, cmdBasename);

        //fmt::print("TID {}: cpu = {}%, user = {}, cmd = {}\n", i.tid, proc_info.getCpuPercent(i), proc_info.getUser(i), proc_info.getCmdBasename(i));
    };

    user_interface.render();
};
