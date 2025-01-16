#pragma once

#include "../event.h"
#include "../../util/geom.h"

#include <glm/gtx/vector_angle.hpp>

#include "../../managers/scene_manager.h"
#include "../event/reposition_gizmo_event.h"
#include "../state/gizmo_matrix_state.h"
#include "../state/face_selection_state.h"
#include "../state/edge_selection_state.h"
#include "../state/mesh_selection_state.h"
#include "../state/vertex_selection_state.h"

/**
 * This listener should be used to listen to reposition gizmo events.
 * Repositioning of the gizmo should happen when we select stuff on the scene.
 * Data should be available on the state manager and should be set by any command or other means.
 */
struct SceneRenderedListener {
    static void listen(const Event& e) {
        try {
            const auto event = dynamic_cast<const RepositionGizmoEvent&>(e);

            if (event.mode == SelectionMode::OBJECTS) {
                // Identity matrix initially
                auto transform = glm::mat4(1.0f);
                // Get selected mesh from state
                if (const auto mesh = MeshSelectionState::get(); mesh != nullptr) {
                    // Get transform from selected mesh
                    transform = mesh->transform.transform;
                }
                // Position gizmo at the transform
                GizmoMatrixState::set(transform);
            }
            else if (event.mode == SelectionMode::VERTICES) {
                // Identity matrix initially
                glm::mat4 transform = glm::mat4(1.0f);
                // Get selected vertices from state
                if (const auto vertices = VertexSelectionState::get(); !vertices.empty()) {
                    // Get the centroid of the selected vertices
                    const glm::vec3 centroid = geom::centroid(VertexSelectionState::get());
                    // Position gizmo at the centroid
                    transform = glm::translate(transform, centroid);
                }
                // Position gizmo at the transform
                GizmoMatrixState::set(transform);
            }
            else if(event.mode == SelectionMode::FACES) {
                // Identity matrix initially
                glm::mat4 transform = glm::mat4(1.0f);
                // Get selected faces from state
                if (const auto faces = FaceSelectionState::get(); !faces.empty()) {
                    // Will hold average center between faces
                    auto averageCentroid = glm::vec3(0.0f);
                    // Iterate over faces
                    for (const auto& face: faces) {
                        // And vertices for each face
                        for (const auto& vertex: face) {
                            // Get positions of vertices
                            averageCentroid += vertex->position;
                        }
                    }
                    // Divide positions by total faces times 3 vertices
                    averageCentroid /= faces.size() * 3;
                    // Apply the translation to the transform
                    transform = glm::translate(transform, averageCentroid);
                }
                // Position gizmo at the transform
                GizmoMatrixState::set(transform);
            }
            else if(event.mode == SelectionMode::EDGES) {
                // Identity matrix initially
                glm::mat4 transform = glm::mat4(1.0f);
                // Get selected edges from state
                const auto edges = EdgeSelectionState::get();
                // Will hold average center between edges
                auto averageCentroid = glm::vec3(0.0f);
                // Iterate over selected edges
                for (const auto& edge: edges) {
                    // Add to the centroid the position of first vertex of the edge
                    averageCentroid += edge.first->position;
                    // Add to the centroid the position of second vertex of the edge
                    averageCentroid += edge.second->position;
                }

                // Divide positions by total edges times 2 vertices per edge
                averageCentroid /= edges.size() * 2;
                // Apply the translation to the transform
                transform = glm::translate(transform, averageCentroid);
                // Position gizmo at the transform
                GizmoMatrixState::set(transform);
            }
        } catch (const std::exception& exception) {
            // Report
        }
    }
};
