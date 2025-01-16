#include "create_scene_object_command.h"

CreateSceneObjectCommand::CreateSceneObjectCommand(const std::shared_ptr<SceneManager>& scene, const  std::shared_ptr<Model>& model) : scene(scene), model(model) {}

bool CreateSceneObjectCommand::execute() {
    if (model == nullptr) {
        return false;
    }

    scene->addModel(model);

    return true;
}

void CreateSceneObjectCommand::undo() {
    scene->getModels().pop_back();
}

void CreateSceneObjectCommand::redo() {
    this->execute();
}