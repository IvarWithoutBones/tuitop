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

    ftxui::Component UserInterface::procEntry(tuitop::proc proc, std::string user, std::string cmd, ftxui::Color cmdHighlightColor, ftxui::Color selectedHighlight) {
        return ftxui::Renderer(bufContainer, [this, proc, cmd, user, cmdHighlightColor, selectedHighlight] {
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
                    | ftxui::color(ftxui::Color(cmdHighlightColor)) | ftxui::flex
            }) | ftxui::bgcolor(selectedHighlight);
        });
    };

    ftxui::Component UserInterface::wrapInputHandler(ftxui::Component component) {
        return ftxui::CatchEvent(component, [this] (ftxui::Event event) {
            if (event == ftxui::Event::Character('q'))
                screen.ExitLoopClosure()();

            if (event == ftxui::Event::Character('j'))
                selectedProc++;
            if (event == ftxui::Event::Character('k'))
                selectedProc--;

            return true;
        });
    };

    void UserInterface::updateProcs(const std::vector<tuitop::proc> &proc_list) {
        int idx = 0;
        for (const tuitop::proc &i : proc_list) {
            addProcess(i, idx);
            idx++;
        };

        processContainer.swap(bufContainer);
        screen.PostEvent(ftxui::Event::Custom);
        bufContainer->DetachAllChildren();
    };

    void UserInterface::addProcess(const tuitop::proc &proc, int index) {
        ftxui::Color selectedHighlight = colors.background;
        ftxui::Color cmdHighlightColor;
        std::string user = proc.user;
        std::string cmd;

        if (index == selectedProc)
            selectedHighlight = colors.selectedHighlight;

        if (user.length() > 10)
            user = user.substr(0, 10) + "-";

        if (!proc.command.empty()) {
            cmd = proc.command;
            cmdHighlightColor = colors.command;
        } else if (!proc.cmdBasename.empty()) {
            cmd = proc.cmdBasename;
            cmdHighlightColor = colors.cmdBasename;
        } else {
            return;
        };

        auto entry = procEntry(proc, user, cmd, cmdHighlightColor, selectedHighlight);
        bufContainer->Add(entry);
    };

    void UserInterface::render() {
        auto renderer = ftxui::Renderer(processContainer, [this] {
            return ftxui::vbox({
                // The bar explaining each colums type
                statusBar(),
                // The currently running processes
                processContainer->Render(),
            }) | ftxui::bgcolor(colors.background);
        });

        auto component = wrapInputHandler(renderer);
        screen.Loop(component);
    };
}
