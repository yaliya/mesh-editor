#pragma once
#include "../command.h"
#include "../../managers/scene_manager.h"

class CreateSceneObjectCommand final : public Command {
    std::shared_ptr<SceneManager> scene;
    std::shared_ptr<Model> model;
public:
    explicit CreateSceneObjectCommand(const std::shared_ptr<SceneManager>& scene, const  std::shared_ptr<Model>& model);

    bool execute() override;
    void undo() override;
    void redo() override;
};
