#include "select_vertex_command.h"

#include "../../managers/event_manager.h"
#include "../../managers/scene_manager.h"

#include "../state/mesh_selection_state.h"
#include "../state/vertex_selection_state.h"
#include "../event/reposition_gizmo_event.h"

SelectVertexCommand::SelectVertexCommand(const std::shared_ptr<SceneManager>& scene): scene(scene) {
    if (MeshSelectionState::get() != nullptr) {
        this->selectVertex(*MeshSelectionState::get());
    }
    else {
        for (const auto& model : scene->getModels()) {
            for (auto& mesh : model->getMeshes()) {
                this->selectVertex(mesh);
                break;
            }
        }
    }
}

/**
 * Vertex selection method on the given mesh
 * Identify the closest vertex to a ray cast from the camera's position.
 */
void SelectVertexCommand::selectVertex(Mesh& mesh) {
    const auto camera = scene->getCamera();

    // Retrieve the mesh's transformation matrix
    const glm::mat4& transform = mesh.transform.getTransform();

    for (Vertex& vertex : mesh.vertices) {
        // Apply the transformation matrix to the vertex position (convert it to world space)
        glm::vec4 transformedVertex = transform * glm::vec4(vertex.position, 1.0f);

        // Compute the vector from the camera to the transformed vertex
        glm::vec3 toVertex = glm::vec3(transformedVertex) - camera->rayOrigin;
        const float t = glm::dot(toVertex, camera->rayDirection);

        if (t < 0.0f) continue;  // Skip if the vertex is behind the camera

        // Compute the closest point on the ray and check if it's within a small threshold
        glm::vec3 closestPointOnRay = camera->rayOrigin + t * camera->rayDirection;
        float distance = glm::length(closestPointOnRay - glm::vec3(transformedVertex));

        if (distance < 0.01f) {  // If the distance is small enough, select the vertex
            selectedVertex = &vertex;
            MeshSelectionState::set(&mesh);
            break;
        }
    }
}

bool SelectVertexCommand::execute() {
    if (!selectedVertex) {
        return false;
    }

    VertexSelectionState::add(selectedVertex);
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::VERTICES);
    return true;
}

void SelectVertexCommand::undo() {
    if (const auto selected = VertexSelectionState::get(); !selected.empty()) {
        VertexSelectionState::pop();
    }

    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::VERTICES);
}

void SelectVertexCommand::redo() {
    this->execute();
}

