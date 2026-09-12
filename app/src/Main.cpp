#include <iostream>
#include <SpecialViewApp.hpp>
#include <engine/core/Engine.hpp>
#include "../../engine/libs/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../../cmake-build-debug/engine/libs/glfw/src/wayland-client-protocol.h"

/**
 * Start here...
 */

int main(int argc, char **argv) {
    auto app = std::make_unique<app::specialViewApp>();
    return app->run(argc, argv);
}
