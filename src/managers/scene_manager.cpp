#include "scene_manager.h"

#include "event_manager.h"
#include "../core/event/reposition_gizmo_event.h"
#include "../core/state/edge_selection_state.h"
#include "../core/state/face_selection_state.h"
#include "../core/state/gizmo_matrix_state.h"
#include "../core/state/mesh_selection_state.h"
#include "../core/state/vertex_selection_state.h"

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::addModel(const std::shared_ptr<Model>& model) {
    models.emplace_back(model);
}

void SceneManager::addCamera(const std::shared_ptr<Camera>& camera) {
    this->camera = camera;
}

std::shared_ptr<Camera> SceneManager::getCamera() {
    return this->camera;
}

void SceneManager::addGrid(const std::shared_ptr<Grid> &grid) {
    this->grid = grid;
}

std::shared_ptr<Grid> SceneManager::getGrid() {
    return this->grid;
}

void SceneManager::setViewport(const int width, const int height) const {
    camera->viewWidth = width;
    camera->viewHeight = height;
}

std::vector<std::shared_ptr<Model>>& SceneManager::getModels() {
    return models;
}

void SceneManager::setSelectionMode(const SelectionMode mode) {
    this->selectionMode = mode;
    EventManager::dispatch<RepositionGizmoEvent>(mode);
}

SelectionMode SceneManager::getSelectionMode() const {
    return this->selectionMode;
}

void SceneManager::clear() {
    GizmoMatrixState::clear();
    MeshSelectionState::clear();
    EdgeSelectionState::clear();
    FaceSelectionState::clear();
    VertexSelectionState::clear();

    selectionMode = SelectionMode::NONE;
    models.clear();
}
