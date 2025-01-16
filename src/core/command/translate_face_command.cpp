#include <map>
#include <unordered_set>
#include <glm/gtc/type_ptr.hpp>

#include "../../util/geom.h"
#include "translate_face_command.h"
#include "../../managers/event_manager.h"
#include "../../managers/scene_manager.h"
#include "../state/face_selection_state.h"
#include "../state/mesh_selection_state.h"
#include "../event/reposition_gizmo_event.h"
#include "../state/gizmo_matrix_state.h"

TranslateFaceCommand::TranslateFaceCommand(const std::shared_ptr<SceneManager>& scene) : scene(scene) {
    stackable = true;
    for (const auto& face : FaceSelectionState::get()) {
        for (const auto& vertex: face) {
            lastPositions[vertex] = vertex->position;
        }
    }
}

bool TranslateFaceCommand::execute() {
    const auto mesh = MeshSelectionState::get();
    const auto gizmo = GizmoMatrixState::get();
    const auto faces = FaceSelectionState::get();

    if (faces.empty() || mesh == nullptr) {
        return false;
    }

    // Calculate the dynamic centroid based on current unique vertex positions
    glm::vec3 dynamicCentroid(0.0f);
    std::unordered_set<Vertex*> uniqueVertices(faces.size() * 6);

    for (const auto& face : faces) {
        for (const auto& vertex: face) {
            uniqueVertices.insert(vertex);
        }
    }

    for (const auto& vertex: uniqueVertices) {
        dynamicCentroid += vertex->position;
    }

    dynamicCentroid /= static_cast<float>(uniqueVertices.size());

    // Calculate the translation offset based on the dynamic centroid
    const glm::vec3 translationOffset = glm::vec3(gizmo[3]) - dynamicCentroid;

    // Move each unique vertex by the translation offset
    for (const auto& vertex : uniqueVertices) {
        vertex->position += translationOffset;
        geom::recalculateNormalsForAffectedFaces(*mesh, vertex->position);
    }

    mesh->bind();
    return true;
}

void TranslateFaceCommand::undo() {
    const auto mesh = MeshSelectionState::get();
    const auto faces = FaceSelectionState::get();

    // Will hold current positions as new last positions
    std::map<Vertex*, glm::vec3> newLastPositions;

    for (const auto& face: faces) {
        for (const auto& vertex: face) {
            newLastPositions[vertex] = vertex->position;
            vertex->position = lastPositions[vertex];
            geom::recalculateNormalsForAffectedFaces(*mesh, vertex->position);
        }
    }

    // Replace last positions
    lastPositions = newLastPositions;
    // Update the buffers
    mesh->bind();
    // Inform the gizmo to reposition
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::FACES);
}

void TranslateFaceCommand::redo() {
    const auto mesh = MeshSelectionState::get();
    const auto faces = FaceSelectionState::get();

    // Will hold current positions as new last positions
    std::map<Vertex*, glm::vec3> newLastPositions;

    for (const auto& face: faces) {
        for (const auto& vertex: face) {
            newLastPositions[vertex] = vertex->position;
            vertex->position = lastPositions[vertex];
            geom::recalculateNormalsForAffectedFaces(*mesh, vertex->position);
        }
    }

    // Replace last positions
    lastPositions = newLastPositions;
    // Update the buffers
    mesh->bind();
    // Inform the gizmo to reposition
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::FACES);
}