#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "select_face_command.h"

#include "../../managers/event_manager.h"
#include "../../managers/scene_manager.h"
#include "../event/reposition_gizmo_event.h"

#include "../state/face_selection_state.h"
#include "../state/mesh_selection_state.h"


SelectFaceCommand::SelectFaceCommand(const std::shared_ptr<SceneManager>& scene) : scene(scene) {
    if (MeshSelectionState::get() != nullptr) {
        this->selectFace(*MeshSelectionState::get());
    }
    else {
        for (const auto& model : scene->getModels()) {
            for (auto& mesh : model->getMeshes()) {
                if (this->selectFace(mesh)) {
                    break;
                }
            }
        }
    }
}

bool SelectFaceCommand::selectFace(Mesh& mesh) {
    constexpr float selectionThreshold = 0.1f;
    auto camera = scene->getCamera();
    float closestDistance = selectionThreshold;

    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        if (i + 2 >= mesh.indices.size()) break;

        const Vertex& v1 = mesh.vertices[mesh.indices[i]];
        const Vertex& v2 = mesh.vertices[mesh.indices[i + 1]];
        const Vertex& v3 = mesh.vertices[mesh.indices[i + 2]];

        glm::vec3 edge1 = v2.position - v1.position;
        glm::vec3 edge2 = v3.position - v1.position;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));
        glm::vec3 faceCentroid = (v1.position + v2.position + v3.position) / 3.0f;

        // Check if face normal points towards the camera
        glm::vec3 viewDirection = glm::normalize(camera->rayOrigin - faceCentroid);
        // Skip back-facing face
        if (glm::dot(faceNormal, viewDirection) <= 0) {
            continue;
        }

        glm::vec3 originToCentroid = faceCentroid - camera->rayOrigin;
        float projectionLength = glm::dot(originToCentroid, camera->rayDirection);
        glm::vec3 projectedPoint = camera->rayOrigin + projectionLength * camera->rayDirection;

        float distance = glm::length(faceCentroid - projectedPoint);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestFace = {&mesh.vertices[mesh.indices[i]], &mesh.vertices[mesh.indices[i + 1]], &mesh.vertices[mesh.indices[i + 2]]};
        }
    }

    if (!closestFace.empty()) {
        MeshSelectionState::set(&mesh);
        return true;
    }

    return false;
}


bool SelectFaceCommand::execute() {
    if (closestFace.empty()) {
        return false;
    }

    FaceSelectionState::add(closestFace);
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::FACES);
    return true;
}

void SelectFaceCommand::undo() {
    FaceSelectionState::pop();
    EventManager::dispatch<RepositionGizmoEvent>(SelectionMode::FACES);
}

void SelectFaceCommand::redo() {
    this->execute();
}

