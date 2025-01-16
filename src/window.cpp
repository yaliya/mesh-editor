#include "window.h"
#include <iostream>

#include "assets/object3d.h"
#include "managers/asset_manager.h"
#include "systems/gui_system.h"
#include "systems/input_system.h"
#include "systems/render_system.h"

Window::Window(const int width, const int height) {
    this->width = width;
    this->height = height;

    // Initialize GLFW
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Set up OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8); // 8x MSAA

    // Create a GLFW window
    window = glfwCreateWindow(1280, 720, "MeshEdit", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glViewport(0, 0, width, height);
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, resizeCallback);
    commands = std::make_shared<CommandManager>();
}

GLFWwindow* Window::getWindow() const {
    return window;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::resizeCallback(GLFWwindow* window, const int width, const int height) {
    glfwSetWindowSize(window, width, height);
    glViewport(0, 0, width, height);

    auto *win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    win->scene->setViewport(width, height);
    win->getSystem<RenderSystem>().reload();
}

void Window::setScene(const std::shared_ptr<SceneManager>& manager) {
    this->scene = manager;
    this->scene->addGrid(std::make_shared<Grid>(128));
    this->scene->addCamera(std::make_shared<Camera>(width, height, 45.0f, 0.01f, 100.f));
}

std::shared_ptr<SceneManager> Window::getScene() const {
    return scene;
}

void Window::setCommandManager(const std::shared_ptr<CommandManager> &manager) {
    commands = manager;
}

std::shared_ptr<CommandManager> Window::getCommandManager() const {
    return commands;
}

void Window::update() {
    getSystem<InputSystem>().update(glfwGetTime());
    getSystem<GUISystem>().update(glfwGetTime());

    glfwPollEvents();
    glfwSwapBuffers(window);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

