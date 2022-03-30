#include <thread>
#include <chrono>
#include <mutex>
#include <fmt/core.h>
#include "proc_info.h"
#include "ui.h"

int main(int argc, const char* argv[]) {
    std::mutex mutex;
    std::condition_variable cond;
    bool shouldRun = true;

    auto proc_info = tuitop::ProcInfo();
    auto user_interface = tuitop::UserInterface();

    // We have to initialise this manually
    const auto proc_list = proc_info.getRunningProcs();
    for (const tuitop::proc &i : proc_info.getRunningProcs()) {
        user_interface.addProcess(i);
    };

    std::jthread procThread([&user_interface, &proc_info, &mutex, &shouldRun, &cond] (std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait_for(lock, std::chrono::milliseconds(750), [&shouldRun] { return !shouldRun; });

            user_interface.updateProcs({proc_info.getRunningProcs()});
        };
    });

    std::thread renderThread([&user_interface] {
        user_interface.render();
    });

    renderThread.join();

    procThread.request_stop();
    shouldRun = false;
    cond.notify_one();

    procThread.join();
};
