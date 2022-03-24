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

    void UserInterface::addProcess(std::string &pid, std::string &user, std::string &cpu_usage, std::string &command) {
        if (command.empty())
            return;

        auto processEntry = ftxui::Renderer(processContainer, [pid, user, cpu_usage, command] {
            return ftxui::hbox({
                ftxui::text(fmt::format(" PID: {}", pid)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 13),
                ftxui::separator(),
                ftxui::text(fmt::format(" User: {} ", user)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::separator(),
                ftxui::text(fmt::format(" CPU: {} ", cpu_usage)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 12),
                ftxui::separator(),
                ftxui::text(fmt::format(" Command: {}", command)) | ftxui::flex
            });
        });

        processContainer->Add(processEntry);
    };
}
