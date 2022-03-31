#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <fmt/core.h>
#include "ui.h"

namespace tuitop {
    UserInterface::Colors colors;

    const void UserInterface::updateProcs(const std::vector<tuitop::proc> &proc_list) {
        bufContainer->DetachAllChildren();

        for (const tuitop::proc &i : proc_list) {
            addProcess(i);
        };

        processContainer.swap(bufContainer);
        screen.PostEvent(ftxui::Event::Custom);
    };

    const void UserInterface::addProcess(const tuitop::proc &proc) {
        std::string cmd;
        auto cmdHighlightColor = ftxui::Color(ftxui::Color::Default);

        if (!proc.command.empty()) {
            cmd = proc.command;
            cmdHighlightColor = colors.command;
        } else if (!proc.cmdBasename.empty()) {
            cmd = proc.cmdBasename;
            cmdHighlightColor = colors.cmdBasename;
        } else {
            return;
        };

        auto processEntry = ftxui::Renderer(processContainer, [proc, cmd, cmdHighlightColor] {
            return ftxui::hbox({
                ftxui::text(proc.pid) | ftxui::align_right | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 8),
                ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 1),
                ftxui::text(proc.user) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::text(proc.cpuPercent) | ftxui::align_right | ftxui::color(colors.cpu) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3),
                ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 2),
                ftxui::text(cmd) | ftxui::color(ftxui::Color(cmdHighlightColor)) | ftxui::flex,
            });
        });

        bufContainer->Add(processEntry);
    };

    const void UserInterface::render() {
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
                }) | ftxui::bgcolor(colors.bar),

                // The currently running processes
                processContainer->Render(),
            }) | ftxui::bgcolor(colors.background);
        });

        auto component = ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
            if (event == ftxui::Event::Character('q'))
                screen.ExitLoopClosure()();

            return true;
         });

        screen.Loop(component);
    };
}
