#include "grid.h"
#include <glad/glad.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "../util/geom.h"

Grid::Grid(int divisions) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> edges;

    const float squareSize = 0.5f; // Each square is 4 units
    const float gridSize = squareSize * divisions; // Total size of the grid

    const float halfGridSize = gridSize / 2.0f;

    // Generate vertices for the grid
    for (int i = 0; i <= divisions; ++i) {
        const float x = -halfGridSize + (static_cast<float>(i) / divisions) * gridSize;

        // Vertical lines
        vertices.emplace_back(glm::vec3(x, 0.0f, -halfGridSize), glm::vec3(0.0f, 1.0f, 0.0f)); // Left
        vertices.emplace_back(glm::vec3(x, 0.0f, halfGridSize), glm::vec3(0.0f, 1.0f, 0.0f)); // Right

        const float z = -halfGridSize + (static_cast<float>(i) / divisions) * gridSize;

        // Horizontal lines
        vertices.emplace_back(glm::vec3(-halfGridSize, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f)); // Back
        vertices.emplace_back(glm::vec3(halfGridSize, 0.0f, z), glm::vec3(0.0f, 1.0f, 0.0f)); // Front
    }

    // Generate indices for the grid lines
    for (int i = 0; i <= divisions; ++i) {
        // Vertical line indices
        indices.push_back(i * 2);     // Left vertex
        indices.push_back(i * 2 + 1); // Right vertex

        // Horizontal line indices
        indices.push_back(divisions * 2 + i * 2);     // Back vertex
        indices.push_back(divisions * 2 + i * 2 + 1); // Front vertex
    }

    // Add mesh created from vertices, indices, and edges
    this->addMesh({vertices, indices, geom::generateEdges(indices)});
}
