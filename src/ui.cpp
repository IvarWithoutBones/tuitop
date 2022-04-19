#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "ui.h"

namespace tuitop {
    UserInterface::Colors colors;

    ftxui::Element UserInterface::filler(int size) {
        return ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, size);
    };

    ftxui::Component UserInterface::inputHandler(ftxui::Component component) {
        return ftxui::CatchEvent(component, [this] (ftxui::Event event) {
            auto containerSize = procContainer->ChildCount() - 1;

            if (event == ftxui::Event::Character('q'))
                screen.ExitLoopClosure()();

            if (event == ftxui::Event::Character('j') || event == ftxui::Event::ArrowDown) {
                if (selectedProc < containerSize)
                    selectedProc++;
                else
                    selectedProc = 0;

                scrollOffset = selectedProc;
            };

            if (event == ftxui::Event::Character('k') || event == ftxui::Event::ArrowUp) {
                if (selectedProc > 0)
                    selectedProc--;
                else
                    selectedProc = containerSize;

                scrollOffset = selectedProc;
            };

            if (event.is_mouse()) {
                if (event.mouse().button == ftxui::Mouse::WheelDown) {
                    if (scrollOffset+3 < containerSize)
                        scrollOffset += 3;
                    else
                        scrollOffset = containerSize;
                }

                if (event.mouse().button == ftxui::Mouse::WheelUp) {
                    if (scrollOffset-3 > 0)
                        scrollOffset -= 3;
                    else
                        scrollOffset = 0;
                }
            };

            return true;
        });
    };

    void UserInterface::updateProcs(const std::vector<tuitop::proc> &proc_list) {
        procList.clear();

        for (const tuitop::proc &proc : proc_list) {
            std::string user = proc.user;
            std::string cmd;

            if (user.length() > 10)
                user = user.substr(0, 10) + "-";

            if (!proc.command.empty()) {
                cmd = proc.command;
            } else if (!proc.cmdBasename.empty()) {
                cmd = proc.cmdBasename;
            } else {
                // If no command is found we want to skip this entry
                return;
            };

            auto entry = procEntry(proc, user, cmd);
            procList.push_back(entry);
        };

        procContainer->DetachAllChildren();
        for (auto &proc : procList) {
            procContainer->Add(proc);
        };

        auto containerSize = procContainer->ChildCount() - 1;

        if (selectedProc > containerSize)
            selectedProc = containerSize;
        if (scrollOffset > containerSize)
            scrollOffset = containerSize;

        screen.PostEvent(ftxui::Event::Custom);
    };

    ftxui::Component UserInterface::procEntry(tuitop::proc proc, std::string user, std::string cmd) {
        return ftxui::Renderer([this, proc, cmd, user] (bool focused) {
            auto bgcolor = focused ? colors.focused : colors.background;
            auto commandColor = (!proc.command.empty()) ? colors.command : colors.cmdBasename;

            return ftxui::hbox({
                ftxui::text(proc.pid)
                    | ftxui::align_right
                    | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 8),
                filler(1),
                ftxui::text(user)
                    | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::text(proc.cpuPercent)
                    | ftxui::align_right
                    | ftxui::color(colors.cpu)
                    | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 3),
                filler(2),
                ftxui::text(cmd)
                    | ftxui::color(ftxui::Color(commandColor))
                    | ftxui::flex
            }) | ftxui::bgcolor(bgcolor);
        });
    };

    ftxui::Element UserInterface::statusBar() {
        return ftxui::hbox({
            filler(5),
            ftxui::text("PID"),
            filler(1),
            ftxui::text("User"),
            filler(8),
            ftxui::text("CPU"),
            filler(2),
            ftxui::text("Command")
        }) | ftxui::bgcolor(colors.bar);
    };

    void UserInterface::render() {
        auto layout = ftxui::Renderer(procContainer, [this] {
            return ftxui::vbox({
                // The bar explaining each colums type
                statusBar(),
                // The currently running processes
                procContainer->Render()
                    | ftxui::focusPosition(0, std::move(scrollOffset))
                    | ftxui::frame
                    | ftxui::flex,
            }) | ftxui::bgcolor(colors.background);
        });

        auto component = inputHandler(layout);
        screen.Loop(component);
    };
}
