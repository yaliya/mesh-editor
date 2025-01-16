#pragma once
#include <glad/glad.h>

#include <memory>
#include <typeindex>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include "systems//base_system.h"
#include "managers/scene_manager.h"
#include "managers/command_manager.h"

class Window : public std::enable_shared_from_this<Window>{
    int width, height;
    GLFWwindow* window{};
    std::shared_ptr<SceneManager> scene;
    std::shared_ptr<CommandManager> commands;
    std::unordered_map<std::type_index, std::unique_ptr<BaseSystem>> systems;

    static void resizeCallback(GLFWwindow* window, int width, int height);
public:
    Window(int width, int height);
    ~Window();

    void update();
    [[nodiscard]] bool shouldClose() const;
    [[nodiscard]] GLFWwindow* getWindow() const;

    void setScene(const std::shared_ptr<SceneManager>& manager);
    std::shared_ptr<SceneManager> getScene() const;

    void setCommandManager(const std::shared_ptr<CommandManager>& manager);
    [[nodiscard]] std::shared_ptr<CommandManager> getCommandManager() const;

    template <typename T>
    T& addSystem() {
        auto system = std::make_unique<T>(shared_from_this());
        systems[typeid(T)] = std::move(system);
        return *static_cast<T*>(systems[typeid(T)].get());
    }

    template <typename T>
    T& getSystem() {
        if (const auto it = systems.find(typeid(T)); it != systems.end()) {
            return *static_cast<T*>(it->second.get());
        }

        throw std::runtime_error("System not found");
    }
};
