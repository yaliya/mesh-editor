#pragma once

#include <memory>

#include "../command.h"
#include "../../managers/scene_manager.h"

class SelectVertexCommand final : public Command {
    std::shared_ptr<SceneManager> scene;
    Vertex* selectedVertex = nullptr;
public:
    explicit SelectVertexCommand(const std::shared_ptr<SceneManager>& scene);
    void selectVertex(Mesh& mesh);
    bool execute() override;
    void undo() override;
    void redo() override;
};
