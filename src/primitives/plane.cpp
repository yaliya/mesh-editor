#include "plane.h"
#include "../types/mesh.h"
#include "../util/geom.h"

Plane::Plane(const float width, const float depth, const unsigned int subdivisionsX, const unsigned int subdivisionsZ) {
    // Calculate the number of vertices
    const unsigned int vertexCountX = subdivisionsX + 1;
    const unsigned int vertexCountZ = subdivisionsZ + 1;

    const float halfWidth = width / 2.0f;
    const float halfDepth = depth / 2.0f;

    std::vector<Vertex> vertices;
    for (unsigned int z = 0; z < vertexCountZ; ++z) {
        for (unsigned int x = 0; x < vertexCountX; ++x) {
            const float xPos = -halfWidth + (static_cast<float>(x) / subdivisionsX) * width;
            const float zPos = -halfDepth + (static_cast<float>(z) / subdivisionsZ) * depth;
            vertices.emplace_back(glm::vec3(xPos, 0.0f, zPos), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int z = 0; z < subdivisionsZ; ++z) {
        for (unsigned int x = 0; x < subdivisionsX; ++x) {
            unsigned int topLeft = z * vertexCountX + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * vertexCountX + x;
            unsigned int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    this->addMesh({vertices, indices, geom::generateEdges(indices)});
}
