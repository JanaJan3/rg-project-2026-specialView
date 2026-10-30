#include <GuiController.hpp>

#include <MainController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include <imgui.h>

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
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto main_controller = engine::core::Controller::get<MainController>();
    graphics->begin_gui();

    const ImGuiWindowFlags fixed_flags = ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
    ImGui::Begin("Golem introduction");
    ImGui::Text("Hi, This is... me. I'm made of stone so I handle impact well.");
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(20, 200), ImGuiCond_Always);
    ImGui::Begin("Ambient Lighting");
    ImGui::Separator();
    ImGui::ColorEdit3("Ambient color", &main_controller->ambient_color()[0]);
    ImGui::SliderFloat("Ambient strength", &main_controller->ambient_strength(), 0.0f, 1.0f);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(20, 320), ImGuiCond_Always);
    ImGui::Begin("Directional Lighting");
    auto &dir_light = main_controller->dir_light();
    ImGui::Checkbox("Directional light enabled", &dir_light.enabled);
    ImGui::ColorEdit3("Directional color", &dir_light.color[0]);
    ImGui::SliderFloat("Directional intensity", &dir_light.intensity, 0.0f, 2.0f);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(20, 460), ImGuiCond_Always);
    ImGui::Begin("Event Chain");
    ImGui::Text("Press T to trigger the event chain");
    if (ImGui::Button("Trigger manually")) {
        main_controller->start_event_chain();
    }
    ImGui::End();

    graphics->end_gui();
}
}// namespace app
