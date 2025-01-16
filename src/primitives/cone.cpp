#include "cone.h"
#include <glad/glad.h>
#include <memory>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

#include "../util/geom.h"

Cone::Cone(float radius, float height, int segments, int verticalDivisions) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> edges;

    // Apex vertex
    vertices.emplace_back(glm::vec3(0.0f, height, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Index 0

    // Generate vertices along the height for vertical divisions
    for (int j = 0; j <= verticalDivisions; ++j) {
        const float h = height * static_cast<float>(j) / verticalDivisions;  // Height level for this ring
        const float r = radius * (1.0f - static_cast<float>(j) / verticalDivisions);  // Radius level for this ring

        for (int i = 0; i < segments; ++i) {
            const float theta = 2.0f * M_PI * i / segments;
            const float x = r * cos(theta);
            const float z = r * sin(theta);

            // Calculate normal for side vertex
            glm::vec3 sideNormal = glm::normalize(glm::vec3(x, radius / height, z));
            vertices.emplace_back(glm::vec3(x, h, z), sideNormal);
        }
    }

    // Base center vertex for triangulation
    const int baseCenterIndex = vertices.size();
    vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)); // Base center vertex

    // Generate vertices for the base
    for (int i = 0; i < segments; ++i) {
        const float theta = 2.0f * M_PI * i / segments;
        const float x = radius * cos(theta);
        const float z = radius * sin(theta);
        glm::vec3 sideNormal = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)); // Normal pointing down
        vertices.emplace_back(glm::vec3(x, 0.0f, z), sideNormal);
    }

    // Indices for the base face (fan) - Ensure counter-clockwise winding
    for (int i = 0; i < segments; ++i) {
        const int nextIndex = (i + 1) % segments; // Wrap around for the last segment
        indices.push_back(baseCenterIndex);  // Center vertex
        indices.push_back(baseCenterIndex + nextIndex + 1); // Next base vertex
        indices.push_back(baseCenterIndex + i + 1); // Current base vertex
    }

    // Indices for the sides with vertical divisions
    for (int j = 0; j < verticalDivisions; ++j) {
        const int ringStart = 1 + j * segments;
        const int nextRingStart = ringStart + segments; // Correctly define nextRingStart

        for (int i = 0; i < segments; ++i) {
            const int nextIndex = (i + 1) % segments;

            // First triangle of the quad
            indices.push_back(ringStart + i);
            indices.push_back(nextRingStart + i);
            indices.push_back(ringStart + nextIndex);

            // Second triangle of the quad
            indices.push_back(ringStart + nextIndex);
            indices.push_back(nextRingStart + i);
            indices.push_back(nextRingStart + nextIndex);
        }
    }

    // Add mesh created from vertices, indices, and edges
    this->addMesh({vertices, indices, geom::generateEdges(indices)});
}
