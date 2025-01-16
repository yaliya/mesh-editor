#pragma once

#include <memory>

class Window;

class BaseSystem {
protected:
    std::shared_ptr<Window> window;
public:
    explicit BaseSystem(const std::shared_ptr<Window>& window) : window(window) {};

    virtual ~BaseSystem() = default;
    virtual void update(const double& delta) = 0;
};
