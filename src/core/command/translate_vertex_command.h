#pragma once

#include <map>
#include <memory>

#include "../command.h"
#include "../../managers/scene_manager.h"

class TranslateVertexCommand final : public Command {
    std::shared_ptr<SceneManager> scene;
    std::map<Vertex*, glm::vec3> lastPositions;
public:
    explicit TranslateVertexCommand(const std::shared_ptr<SceneManager>& scene);
    bool execute() override;
    void undo() override;
    void redo() override;
};
