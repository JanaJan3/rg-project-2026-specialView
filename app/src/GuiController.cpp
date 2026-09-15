//
// Created by ana on 9/14/26.
//

#include "../include/GuiController.hpp"

#include <imgui.h>
#include <MainController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>

namespace app {
    void GUIController::initialize() {
        set_enable(false);
    }

    void GUIController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_B).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
        }
    }

    void GUIController::draw() {
        auto graphics        = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto main_controller = engine::core::Controller::get<MainController>();
        graphics->begin_gui();

        ImGui::Begin("Golem introduction");
        ImGui::Text("Hi, This is... me. I'm made of stone so I handle impact well.");
        ImGui::End();

        ImGui::Begin("Ambient Lighting");
        ImGui::Separator();
        ImGui::ColorEdit3("Ambient color", &main_controller->ambient_color()[0]);
        ImGui::SliderFloat("Ambient strength", &main_controller->ambient_strength(), 0.0f, 1.0f);
        ImGui::End();

        ImGui::Begin("Directional Lighting");
        auto &dir_light = main_controller->dir_light();
        ImGui::Checkbox("Directional light enabled", &dir_light.enabled);
        ImGui::ColorEdit3("Directional color", &dir_light.color[0]);
        ImGui::SliderFloat("Directional intensity", &dir_light.intensity, 0.0f, 2.0f);
        ImGui::End();

        graphics->end_gui();
    }
} // app
