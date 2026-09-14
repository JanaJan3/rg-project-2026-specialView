//
// Created by ana on 9/12/26.
//

#include "../include/SpecialViewApp.hpp"

#include <GuiController.hpp>
#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void specialViewApp::app_setup() {
        spdlog::info("App setup completed");
        auto mainController = register_controller<app::MainController>();
        auto guiController  = register_controller<app::GUIController>();
        mainController->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        mainController->before(guiController);
    }
} // app
