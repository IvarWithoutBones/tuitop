#include <vector>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <proc/readproc.h>
#include "common.h"

namespace tuitop {
    class UserInterface {
        private:
            ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
            ftxui::Component processContainer = ftxui::Container::Vertical({});
        public:
            void render();
            void addProcess(const tuitop::proc&);
    };
}
