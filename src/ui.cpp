#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <fmt/core.h>
#include "ui.h"

namespace tuitop {
    void UserInterface::updateProcs(std::vector<tuitop::proc> proc_list) {
        processContainer->DetachAllChildren();

        for (tuitop::proc &i : proc_list) {
            addProcess(i);
        };

        screen.PostEvent(ftxui::Event::Custom);
    };

    const void UserInterface::addProcess(const tuitop::proc &proc) {
        std::string cmd;
        auto cmdHighlightColor = ftxui::Color(ftxui::Color::Default);

        if (!proc.command.empty()) {
            cmd = proc.command;
            cmdHighlightColor = ftxui::Color(200, 208, 54); // Green highlight
        } else if (!proc.cmdBasename.empty()) {
            cmd = proc.cmdBasename;
            cmdHighlightColor = ftxui::Color(97, 154, 194); // Blue highlight
        } else {
            return;
        };

        auto processEntry = ftxui::Renderer(processContainer, [&proc, cmd, cmdHighlightColor] {
            return ftxui::hbox({
                ftxui::text(proc.pid) | ftxui::align_right | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 8),
                ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 1),
                ftxui::text(proc.user) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::text(proc.cpuPercent) | ftxui::align_right | ftxui::color(ftxui::Color(252, 193, 73)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3),
                ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 2),
                ftxui::text(cmd) | ftxui::color(ftxui::Color(cmdHighlightColor)) | ftxui::flex,
            });
        });

        processContainer->Add(processEntry);
    };

    void const UserInterface::render() {
        auto renderer = ftxui::Renderer(processContainer, [this] {
            return ftxui::vbox({
                // The bar explaining each colums type
                ftxui::hbox({
                    ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5),
                    ftxui::text("PID"),
                    ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 1),
                    ftxui::text("User"),
                    ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 8),
                    ftxui::text("CPU"),
                    ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 2),
                    ftxui::text("Command"),
                }) | ftxui::bgcolor(barColor),

                // The currently running processes
                processContainer->Render(),
            }) | ftxui::bgcolor(backgroundColor);
        });

        auto component = ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
            if (event == ftxui::Event::Character('q'))
                screen.ExitLoopClosure()();

            return true;
         });

        screen.Loop(component);
    };
}
