#pragma once

#include <map>
#include <memory>

#include "../command.h"
#include "../../managers/scene_manager.h"

class TranslateMeshCommand : public Command {
    std::shared_ptr<SceneManager> scene;
public:
    explicit TranslateMeshCommand(const std::shared_ptr<SceneManager>& scene);
    bool execute() override;
    void undo() override;
    void redo() override;
    void done() override;
};
