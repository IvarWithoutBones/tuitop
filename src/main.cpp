#include <fmt/core.h>
#include "proc_info.h"
#include "ui.h"

int main(int argc, const char* argv[]) {
    auto proc_info = tuitop::Proc();
    auto user_interface = tuitop::UserInterface();

    auto proc_list = proc_info.getRunningProcs();

    for (tuitop::proc &i : proc_list) {
        user_interface.addProcess(i);
    };

    user_interface.render();
};
