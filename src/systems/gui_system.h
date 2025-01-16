#pragma once
#include "base_system.h"

class GUISystem final : public BaseSystem {
public:
    explicit GUISystem(const std::shared_ptr<Window>& window);
    void update(const double& deltaTime) override;
};
