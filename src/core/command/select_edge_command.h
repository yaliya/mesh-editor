#pragma once

#include <memory>
#include <vector>

#include "../command.h"
#include "../../managers/scene_manager.h"

class SelectEdgeCommand final : public Command {
    std::shared_ptr<SceneManager> scene;
    std::pair<Vertex*, Vertex*> selectedEdge;

    unsigned int selectedEdgeVao{};
    unsigned int selectedEdgeVbo{};
    bool selectEdge(Mesh& mesh);
public:
    explicit SelectEdgeCommand(const std::shared_ptr<SceneManager>& scene);
    bool execute() override;
    void undo() override;
    void redo() override;
};
