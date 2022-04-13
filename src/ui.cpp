#include <string>
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <fmt/core.h>
#include "ui.h"

namespace tuitop {
    UserInterface::Colors colors;

    ftxui::Element UserInterface::filler(int size) {
        return ftxui::filler() | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, size);
    };

    ftxui::Component UserInterface::inputHandler(ftxui::Component component) {
        return ftxui::CatchEvent(component, [this] (ftxui::Event event) {
            if (event == ftxui::Event::Character('q'))
                screen.ExitLoopClosure()();

            if (event == ftxui::Event::Character('j') || event == ftxui::Event::ArrowDown)
                selectedProc++;
            if (event == ftxui::Event::Character('k') || event == ftxui::Event::ArrowUp)
                selectedProc--;

            return true;
        });
    };

    void UserInterface::updateProcs(const std::vector<tuitop::proc> &proc_list) {
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
                // If no command is found we dont want to show it in the UI
                return;
            };

            auto entry = procEntry(proc, user, cmd);
            procBufContainer->Add(entry);
        };

        procContainer.swap(procBufContainer);
        screen.PostEvent(ftxui::Event::Custom);
        procBufContainer->DetachAllChildren();
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
        auto renderer = ftxui::Renderer(procContainer, [this] {
            return ftxui::vbox({
                // The bar explaining each colums type
                statusBar(),
                // The currently running processes
                procContainer->Render(),
            }) | ftxui::bgcolor(colors.background);
        });

        auto component = inputHandler(renderer);
        screen.Loop(component);
    };
}
