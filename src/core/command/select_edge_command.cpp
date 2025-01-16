#include "select_edge_command.h"

#include "../../managers/event_manager.h"
#include "../state/edge_selection_state.h"
#include "../state/mesh_selection_state.h"
#include "../event/reposition_gizmo_event.h"

SelectEdgeCommand::SelectEdgeCommand(const std::shared_ptr<SceneManager> &scene) : scene(scene) {
    if (MeshSelectionState::get() != nullptr) {
        this->selectEdge(*MeshSelectionState::get());
    }
    else {
        for (const auto& model : scene->getModels()) {
            for (auto& mesh : model->getMeshes()) {
                if (this->selectEdge(mesh)) {
                    break;
                }
            }
        }
    }
}

bool SelectEdgeCommand::selectEdge(Mesh &mesh) {
    constexpr float selectionThreshold = 0.1f; // Adjust based on your scene scale
    const auto camera = scene->getCamera();
    float closestDistance = selectionThreshold;

    std::pair<Vertex*, Vertex*> closestEdge = {nullptr, nullptr};

    for (size_t i = 0; i < mesh.edges.size(); i += 2) {
        Vertex& v1 = mesh.vertices[mesh.edges[i]];
        Vertex& v2 = mesh.vertices[mesh.edges[i + 1]];

        // Edge vector
        glm::vec3 edgeDirection = v2.position - v1.position;
        glm::vec3 rayDirNormalized = glm::normalize(camera->rayDirection);

        // Define vector between the ray origin and the first vertex of the edge
        glm::vec3 originToEdge = v1.position - camera->rayOrigin;

        // Compute dot products needed for the closest approach formula
        float a = glm::dot(edgeDirection, edgeDirection);
        float b = glm::dot(edgeDirection, rayDirNormalized);
        float c = glm::dot(rayDirNormalized, rayDirNormalized);
        float d = glm::dot(edgeDirection, originToEdge);
        float e = glm::dot(rayDirNormalized, originToEdge);

        // Compute the parameters along the edge and ray for the closest points
        float denominator = a * c - b * b;
        float tEdge = (b * e - c * d) / denominator;
        float tRay = (a * e - b * d) / denominator;

        // Clamp tEdge to ensure it lies within the edge segment
        tEdge = glm::clamp(tEdge, 0.0f, 1.0f);

        // Closest point on the edge segment
        glm::vec3 closestPointOnEdge = v1.position + tEdge * edgeDirection;

        // Closest point on the ray
        glm::vec3 closestPointOnRay = camera->rayOrigin + tRay * rayDirNormalized;

        // Calculate the perpendicular distance between the closest points
        float distance = glm::length(closestPointOnEdge - closestPointOnRay);

        // If the edge is within the selection threshold and is the closest found so far
        if (distance < closestDistance) {
            closestDistance = distance;
            closestEdge = std::make_pair(&v1, &v2);
        }
    }

    if (closestEdge.first != nullptr && closestEdge.second != nullptr) {
        selectedEdge = closestEdge;
        MeshSelectionState::set(&mesh);
        return true;
    }

    return false;
}

bool SelectEdgeCommand::execute() {
    if (selectedEdge.first == nullptr || selectedEdge.second == nullptr) {
        return false;
    }

    EdgeSelectionState::add(selectedEdge);
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::EDGES);

    return true;
}

void SelectEdgeCommand::undo() {
    EdgeSelectionState::pop();
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::EDGES);
}

void SelectEdgeCommand::redo() {
    this->execute();
}
