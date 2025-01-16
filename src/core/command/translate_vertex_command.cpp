#include "translate_vertex_command.h"

#include <glm/gtc/type_ptr.hpp>

#include "../../util/geom.h"
#include "../../managers/event_manager.h"
#include "../state/gizmo_matrix_state.h"
#include "../state/mesh_selection_state.h"
#include "../event/reposition_gizmo_event.h"
#include "../state/vertex_selection_state.h"

TranslateVertexCommand::TranslateVertexCommand(const std::shared_ptr<SceneManager> &scene) : scene(scene) {
    stackable = true;
    // Let's keep an array of vertex positions as the last position for undo
    for(const auto& vertex : VertexSelectionState::get()) {
        lastPositions[vertex] = vertex->position;
    }
}

bool TranslateVertexCommand::execute() {
    // Get gizmo state from state manager. Should have been applied by the select vertex command
    auto gizmo = GizmoMatrixState::get();
    // Get selected mesh state from state manager. Should have been applied by the select vertex command
    const auto mesh = MeshSelectionState::get();
    // Get selected vertices state from state manager. Should have been applied by the select vertex command
    const auto vertices = VertexSelectionState::get();

    // But in any case we have no references to any of them just exit this command.
    if (vertices.empty() || mesh == nullptr) {
        return false;
    }

    // Get gizmo translation
    const auto translation = glm::vec3(gizmo[3]);
    // Calculate the centroid from the selected vertices
    const glm::vec3 centroid = geom::centroid(vertices);
    // Move the gizmo to the centroid of the selected vertices
    gizmo[3] = glm::vec4(centroid, 1.0f);
    // Calculate how much the gizmo has moved
    const glm::vec3 translated = translation - centroid;

    for (auto& vertex : vertices) {
        // Apply translation
        vertex->position += translated;
        // Recalculate the normals of the affected faces on the mesh based on the positions
        geom::recalculateNormalsForAffectedFaces(*mesh, vertex->position);
    }
    // Update the buffers
    mesh->bind();
    // No need to reposition gizmo here as it moves as we drag it
    return true;
}

void TranslateVertexCommand::undo() {
    const auto mesh = MeshSelectionState::get();
    const auto vertices = VertexSelectionState::get();

    // Will hold current positions as new last positions
    std::map<Vertex*, glm::vec3> newLastPositions;

    for (const auto& vertex: vertices) {
        newLastPositions[vertex] = vertex->position;
        vertex->position = lastPositions[vertex];
        geom::recalculateNormalsForAffectedFaces(*mesh, vertex->position);
    }

    // Replace last positions
    lastPositions = newLastPositions;
    // Update the buffers
    mesh->bind();
    // Inform the gizmo to reposition
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::VERTICES);
}

void TranslateVertexCommand::redo() {
    const auto mesh = MeshSelectionState::get();
    const auto vertices = VertexSelectionState::get();

    // Will hold current positions as new last positions
    std::map<Vertex*, glm::vec3> newLastPositions;

    for (const auto& vertex: vertices) {
        newLastPositions[vertex] = vertex->position;
        vertex->position = lastPositions[vertex];
        geom::recalculateNormalsForAffectedFaces(*mesh, vertex->position);
    }

    // Replace last positions
    lastPositions = newLastPositions;
    // Update the buffers
    mesh->bind();
    // Inform the gizmo to reposition
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::VERTICES);
}
