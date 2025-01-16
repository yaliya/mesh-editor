#pragma once
#include "base_system.h"

class InputSystem final : public BaseSystem {
public:
    explicit InputSystem(const std::shared_ptr<Window>& window) : BaseSystem(window) {};
    void update(const double& deltaTime) override;
};
