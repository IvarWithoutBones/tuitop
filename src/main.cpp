#include <thread>
#include <chrono>
#include <mutex>
#include "proc_info.h"
#include "ui.h"

int main(int argc, const char* argv[]) {
    auto proc_info = tuitop::ProcInfo();
    auto initial_procs = proc_info.getProcs();
    auto user_interface = tuitop::UserInterface(initial_procs);

    std::mutex mutex;
    std::condition_variable cond;
    bool shouldRun = true;

    std::thread procThread([&user_interface, &proc_info, &mutex, &shouldRun, &cond] {
        while (shouldRun) {
            std::unique_lock<std::mutex> lock(mutex);
            cond.wait_for(lock, std::chrono::milliseconds(500), [&shouldRun] { return !shouldRun; });

            user_interface.updateProcs({proc_info.getProcs()});
        };
    });

    std::thread renderThread([&user_interface] {
        user_interface.render();
    });

    renderThread.join();
    shouldRun = false;
    cond.notify_one();
    procThread.join();
};
