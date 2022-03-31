#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include "common.h"

namespace tuitop {
    class UserInterface {
        private:
            void addProcess(const tuitop::proc&, int index);
            int selectedProc = 0;

            ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
            ftxui::Element filler(int);
            ftxui::Element statusBar();
            ftxui::Component procEntry(tuitop::proc, std::string, ftxui::Color, ftxui::Color);
            ftxui::Component wrapInputHandler(ftxui::Component);

            // The rendered container
            ftxui::Component processContainer = ftxui::Container::Vertical({}, &selectedProc);
            // Gets swapped with processContainer once all data is ready
            ftxui::Component bufContainer = ftxui::Container::Vertical({}, &selectedProc);
        public:
            void updateProcs(const std::vector<tuitop::proc>&);
            void render();

            UserInterface(std::vector<tuitop::proc> &procs) {
                updateProcs(procs);
            };

            struct Colors {
                ftxui::Color background = ftxui::Color(40, 40, 40);
                ftxui::Color bar = ftxui::Color(68, 68, 68);
                ftxui::Color command = ftxui::Color(200, 208, 54);
                ftxui::Color cmdBasename =  ftxui::Color(97, 154, 194);
                ftxui::Color cpu = ftxui::Color(252, 193, 73);
                ftxui::Color selectedHighlight = ftxui::Color(41, 59, 68);
            };
    };
}
