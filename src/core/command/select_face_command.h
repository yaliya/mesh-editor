#pragma once

#include <memory>

#include "../command.h"
#include "../../managers/scene_manager.h"

class SelectFaceCommand final : public Command {
    std::shared_ptr<SceneManager> scene;
    bool selectFace(Mesh& mesh);
    std::vector<Vertex*> closestFace;
public:
    explicit SelectFaceCommand(const std::shared_ptr<SceneManager>& scene);
    bool execute() override;
    void undo() override;
    void redo() override;
};
