//
// Created by ana on 9/12/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLLER_HPP
#define MATF_RG_PROJECT_MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <glm/glm.hpp>

namespace app {
    struct DirectionalLight {
        glm::vec3 direction{-0.3f, -1.0f, -0.2f};
        glm::vec3 color{1.0f, 1.0f, 0.95f};
        float intensity{0.6f};
        bool enabled{true};
    };

    struct PointLight {
        glm::vec3 position{2.5f, 0.0f, -3.0f};
        glm::vec3 color{0.3f, 0.7f, 1.0f};
        float intensity{1.5f};
        float constant{1.0f};
        float linear{0.09f};
        float quadratic{0.032f};
    };

    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void draw_golem();

        void update_camera();

        void update() override;

        void begin_draw() override;

        void draw_skybox();

        void draw_enchanted_crystal();

        void draw_floating_stone();

        void draw() override;

        void end_draw() override;

        DirectionalLight m_dir_light;
        glm::vec3 m_ambient_color{1.0f, 1.0f, 1.0f};
        float m_ambient_strength{0.25f};
        PointLight m_point_light;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

        DirectionalLight &dir_light() {
            return m_dir_light;
        }

        glm::vec3 &ambient_color() {
            return m_ambient_color;
        }

        float &ambient_strength() {
            return m_ambient_strength;
        }

        PointLight &point_light() {
            return m_point_light;
        }
    };
} // app

#endif //MATF_RG_PROJECT_MAINCONTROLLER_HPP
