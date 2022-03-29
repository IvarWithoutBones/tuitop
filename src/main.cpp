#include <thread>
#include <chrono>
#include <fmt/core.h>
#include "proc_info.h"
#include "ui.h"

int main(int argc, const char* argv[]) {
    auto proc_info = tuitop::ProcInfo();
    auto user_interface = tuitop::UserInterface();

    auto proc_list = proc_info.getRunningProcs();

    for (tuitop::proc &i : proc_list) {
        user_interface.addProcess(i);
    };

    std::thread procThread([&user_interface, &proc_info] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(750));
            user_interface.updateProcs({proc_info.getRunningProcs()});
        };
    });

    std::thread renderThread([&user_interface] {
        user_interface.render();
    });

    procThread.detach();
    renderThread.join();
};
