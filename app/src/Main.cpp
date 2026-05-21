#include <iostream>
#include <engine/core/Engine.hpp>
#include "../../engine/libs/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

/**
 * Start here...
 */

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void update(GLFWwindow *window);

int main(int argc, char **argv) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create a window!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        update(window);
        glfwSwapBuffers(window);
    }

    return 0;
}

void update(GLFWwindow *window) {
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

