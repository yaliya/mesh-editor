#include "translate_mesh_command.h"

#include <iostream>
#include <map>
#include <unordered_set>
#include <glm/gtc/type_ptr.hpp>

#include "../../util/geom.h"
#include "../../managers/event_manager.h"
#include "../../managers/scene_manager.h"
#include "../state/face_selection_state.h"
#include "../state/mesh_selection_state.h"
#include "../event/reposition_gizmo_event.h"
#include "../state/gizmo_matrix_state.h"
#include "../state/vertex_selection_state.h"

TranslateMeshCommand::TranslateMeshCommand(const std::shared_ptr<SceneManager>& scene) : scene(scene) {

}

bool TranslateMeshCommand::execute() {
    const auto mesh = MeshSelectionState::get();
    const auto gizmo = GizmoMatrixState::get();

    // Apply translation to the mesh's transformation matrix
    mesh->transform.translate(gizmo[3]);

    return true;
}

void TranslateMeshCommand::done() {
    const auto mesh = MeshSelectionState::get();

    // Now update the selected vertices' positions in world space
    for (auto& vertex : mesh->vertices) {
        // Apply the transformation matrix to the vertex position
        glm::vec4 transformedPos = mesh->transform.getTransform() * glm::vec4(vertex.position, 1.0f);
        vertex.position = glm::vec3(transformedPos); // Convert back to glm::vec3
    }

    mesh->bind();
}

void TranslateMeshCommand::undo() {
    //
}

void TranslateMeshCommand::redo() {
    //
}