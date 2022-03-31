#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "common.h"

namespace tuitop {
    class UserInterface {
        private:
            ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

            // The rendered container
            ftxui::Component processContainer = ftxui::Container::Vertical({});
            // Gets swapped with processContainer once all data is ready
            ftxui::Component bufContainer = ftxui::Container::Vertical({});
        public:
            const void addProcess(const tuitop::proc&);
            const void updateProcs(const std::vector<tuitop::proc>&);
            const void render();

            struct Colors {
                ftxui::Color background = ftxui::Color(40, 40, 40);
                ftxui::Color bar = ftxui::Color(68, 68, 68);
                ftxui::Color command = ftxui::Color(200, 208, 54);
                ftxui::Color cmdBasename =  ftxui::Color(97, 154, 194);
                ftxui::Color cpu = ftxui::Color(252, 193, 73);
            };

            UserInterface(std::vector<tuitop::proc> &procs) {
                updateProcs(procs);
            };
    };
}
