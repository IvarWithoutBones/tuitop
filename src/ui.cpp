#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <fmt/core.h>
#include "ui.h"

namespace tuitop {
    void const UserInterface::render() {
        auto renderer = ftxui::Renderer(processContainer, [this] {
            return processContainer->Render() | ftxui::border;
        });

        screen.Loop(renderer);
    };

    void UserInterface::updateProcs(std::vector<tuitop::proc> proc_list) {
        processContainer->DetachAllChildren();

        for (tuitop::proc &i : proc_list) {
            addProcess(i);
        };

        screen.PostEvent(ftxui::Event::Custom);
    };

    const void UserInterface::addProcess(const tuitop::proc &proc) {
        std::string cmd;

        if (!proc.command.empty()) {
            cmd = proc.command;
        } else if (!proc.cmdBasename.empty()) {
            cmd = proc.cmdBasename;
        } else {
            return;
        };

        auto processEntry = ftxui::Renderer(processContainer, [&proc, cmd] {
            return ftxui::hbox({
                ftxui::text(fmt::format(" PID: {}", proc.pid)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 13),
                ftxui::separator(),
                ftxui::text(fmt::format(" User: {} ", proc.user)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::separator(),
                ftxui::text(fmt::format(" CPU: {}% ", proc.cpuPercent)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::separator(),
                ftxui::text(fmt::format(" Command: {}", cmd)) | ftxui::flex
            });
        });

        processContainer->Add(processEntry);
    };
}
