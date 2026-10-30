#include <MainController.hpp>

#include <GuiController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (!gui_controller->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainController::initialize() {
    spdlog::info("MainController initialized");
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
}

bool MainController::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainController::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_T).state() == engine::platform::Key::State::JustPressed) {
        start_event_chain();
    }
}

void MainController::start_event_chain() {
    if (m_chain_state != EventChainState::Idle) {
        return;
    }
    spdlog::info("Event chain started (ACTION: pressed key T or triggered manually).");
    m_chain_state = EventChainState::WaitingForEventA;
    m_chain_timer = 0.0f;
}

void MainController::update_event_chain() {
    if (m_chain_state == EventChainState::Idle || m_chain_state == EventChainState::Done) {
        return;
    }
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    m_chain_timer += platform->dt();

    if (m_chain_state == EventChainState::WaitingForEventA && m_chain_timer >= 2.0f) {
        spdlog::info("EVENT_A triggered: crystal awakens, point light intensifies.");
        m_point_light.color = glm::vec3(0.4f, 0.9f, 1.0f);
        m_point_light.intensity = 3.0f;
        m_chain_state = EventChainState::WaitingForEventB;
        m_chain_timer = 0.0f;
    } else if (m_chain_state == EventChainState::WaitingForEventB && m_chain_timer >= 3.0f) {
        spdlog::info("EVENT_B triggered: golem reacts, directional light dims.");
        m_dir_light.intensity = 0.2f;
        m_chain_state = EventChainState::Done;
    }
}

void MainController::update_camera() {
    auto gui_controller = engine::core::Controller::get<GUIController>();
    if (gui_controller->is_enabled()) {
        return;
    }
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    float dt = platform->dt();
    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
}

void MainController::update() {
    update_camera();
    update_event_chain();
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("forgotten_ruins");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->draw_skybox(shader, skybox);
}

void MainController::draw_golem() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *stonegolem = resources->model("stonegolem");

    engine::resources::Shader *shader = resources->shader("light");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
    model = glm::scale(model, glm::vec3(0.003f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", graphics->camera()->Position);

    shader->set_vec3("ambientColor", m_ambient_color);
    shader->set_float("ambientStrength", m_ambient_strength);

    shader->set_vec3("dirLight_direction", m_dir_light.direction);
    shader->set_vec3("dirLight_color", m_dir_light.color);
    shader->set_float("dirLight_intensity", m_dir_light.intensity);
    shader->set_bool("dirLight_enabled", m_dir_light.enabled);
    stonegolem->draw(shader);
}

void MainController::draw_enchanted_crystal() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *enchanted_crystal = resources->model("enchanted_crystal");

    engine::resources::Shader *shader = resources->shader("point_dir_ambient");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(3.0f, 0.1f, -3.0f));
    model = glm::scale(model, glm::vec3(0.3f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", graphics->camera()->Position);

    shader->set_vec3("ambientColor", m_ambient_color);
    shader->set_float("ambientStrength", m_ambient_strength);

    shader->set_vec3("dirLight_direction", m_dir_light.direction);
    shader->set_vec3("dirLight_color", m_dir_light.color);
    shader->set_float("dirLight_intensity", m_dir_light.intensity);
    shader->set_bool("dirLight_enabled", m_dir_light.enabled);

    shader->set_vec3("pointLight_position", m_point_light.position);
    shader->set_vec3("pointLight_color", m_point_light.color);
    shader->set_float("pointLight_intensity", m_point_light.intensity);
    shader->set_float("pointLight_constant", m_point_light.constant);
    shader->set_float("pointLight_linear", m_point_light.linear);
    shader->set_float("pointLight_quadratic", m_point_light.quadratic);

    enchanted_crystal->draw(shader);
}

void MainController::draw_floating_stone() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *floating_stone = resources->model("floating_stone");

    engine::resources::Shader *shader = resources->shader("point_dir_ambient");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.5f, -1.3f, -3.0f));
    model = glm::scale(model, glm::vec3(3.0f));
    //model           = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->set_mat4("model", model);
    shader->set_vec3("viewPos", graphics->camera()->Position);

    shader->set_vec3("ambientColor", m_ambient_color);
    shader->set_float("ambientStrength", m_ambient_strength);

    shader->set_vec3("dirLight_direction", m_dir_light.direction);
    shader->set_vec3("dirLight_color", m_dir_light.color);
    shader->set_float("dirLight_intensity", m_dir_light.intensity);
    shader->set_bool("dirLight_enabled", m_dir_light.enabled);

    shader->set_vec3("pointLight_position", m_point_light.position);
    shader->set_vec3("pointLight_color", m_point_light.color);
    shader->set_float("pointLight_intensity", m_point_light.intensity);
    shader->set_float("pointLight_constant", m_point_light.constant);
    shader->set_float("pointLight_linear", m_point_light.linear);
    shader->set_float("pointLight_quadratic", m_point_light.quadratic);

    floating_stone->draw(shader);
}

void MainController::draw() {
    draw_golem();
    draw_enchanted_crystal();
    draw_floating_stone();
    draw_skybox();
}

void MainController::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}
}// namespace app
