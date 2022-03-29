#include <string>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <fmt/core.h>
#include "ui.h"

namespace tuitop {
    void UserInterface::render() {
        auto renderer = ftxui::Renderer(processContainer, [this] {
            return processContainer->Render() | ftxui::border;
        });

        screen.Loop(renderer);
    };

    void UserInterface::addProcess(const tuitop::proc &proc) {
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
