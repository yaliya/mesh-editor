#include "cube.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "../util/geom.h"

Cube::Cube(const float sideLength, const int subdivisions) {
    const float half = sideLength / 2.0f;
    const float step = sideLength / subdivisions;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> edges;

    // Create a 3D grid of vertex positions
    std::vector grid(subdivisions + 1, std::vector(subdivisions + 1, std::vector<int>(subdivisions + 1, -1)));

    // Lambda to add a vertex to the grid if it doesn't exist and return its index
    auto getVertexIndex = [&](const glm::vec3& pos, const glm::vec3& normal) -> int {
        int i = static_cast<int>((pos.x + half) / step);
        int j = static_cast<int>((pos.y + half) / step);
        int k = static_cast<int>((pos.z + half) / step);

        if (grid[i][j][k] == -1) {
            grid[i][j][k] = vertices.size();
            vertices.emplace_back(pos, normal);  // Create vertex with initial normal
        }

        return grid[i][j][k];
    };

    // Lambda to add a quad using shared vertices from the grid
    auto addQuad = [&](const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, const glm::vec3& faceNormal) {
        int idx1 = getVertexIndex(p1, faceNormal);
        int idx2 = getVertexIndex(p2, faceNormal);
        int idx3 = getVertexIndex(p3, faceNormal);
        int idx4 = getVertexIndex(p4, faceNormal);

        indices.insert(indices.end(), {idx1, idx2, idx4}); // First triangle: p1, p2, p4
        indices.insert(indices.end(), {idx2, idx3, idx4}); // Second triangle: p2, p3, p4

        // Accumulate the face normal to the vertex normals
        vertices[idx1].normal += faceNormal;
        vertices[idx2].normal += faceNormal;
        vertices[idx3].normal += faceNormal;
        vertices[idx4].normal += faceNormal;
    };

    // Generate each face using shared vertices from the grid
    for (int i = 0; i < subdivisions; ++i) {
        for (int j = 0; j < subdivisions; ++j) {
            float x = -half + i * step;
            float y = -half + j * step;

            // Front face (+Z)
            addQuad(glm::vec3(x, y, half), glm::vec3(x + step, y, half),
                    glm::vec3(x + step, y + step, half), glm::vec3(x, y + step, half),
                    glm::vec3(0.0f, 0.0f, 1.0f));

            // Back face (-Z)
            addQuad(glm::vec3(x, y, -half), glm::vec3(x, y + step, -half),
                    glm::vec3(x + step, y + step, -half), glm::vec3(x + step, y, -half),
                    glm::vec3(0.0f, 0.0f, -1.0f));

            // Left face (-X)
            addQuad(glm::vec3(-half, y, x), glm::vec3(-half, y + step, x),
                    glm::vec3(-half, y + step, x + step), glm::vec3(-half, y, x + step),
                    glm::vec3(-1.0f, 0.0f, 0.0f));

            // Right face (+X)
            addQuad(glm::vec3(half, y, x), glm::vec3(half, y, x + step),
                    glm::vec3(half, y + step, x + step), glm::vec3(half, y + step, x),
                    glm::vec3(1.0f, 0.0f, 0.0f));

            // Top face (+Y)
            addQuad(glm::vec3(x, half, y), glm::vec3(x, half, y + step),
                    glm::vec3(x + step, half, y + step), glm::vec3(x + step, half, y),
                    glm::vec3(0.0f, 1.0f, 0.0f));

            // Bottom face (-Y)
            addQuad(glm::vec3(x, -half, y), glm::vec3(x + step, -half, y),
                    glm::vec3(x + step, -half, y + step), glm::vec3(x, -half, y + step),
                    glm::vec3(0.0f, -1.0f, 0.0f));
        }
    }

    this->addMesh({vertices, indices, geom::generateEdges(indices)});
}
