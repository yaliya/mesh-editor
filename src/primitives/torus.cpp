#include "torus.h"

#include "../util/geom.h"

Torus::Torus(const int rings, const int sectors, const float gap, const float tube) {
    const float R = gap;     // Radius from the center to the middle of the tube
    const float r = tube;     // Radius of the tube

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> edges;

    // Generate vertices
    for (int i = 0; i <= rings; ++i) {
        float y = r * sin(glm::radians((float)i / rings * 360.0f));
        float circleRadius = R + r * cos(glm::radians((float)i / rings * 360.0f));

        for (int j = 0; j <= sectors; ++j) {
            float x = circleRadius * cos(glm::radians((float)j / sectors * 360.0f));
            float z = circleRadius * sin(glm::radians((float)j / sectors * 360.0f));
            vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(x, y, z))); // You can replace normal with a fixed value if needed
        }
    }

    // Generate indices and edges
    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int first = (i * (sectors + 1)) + j;
            int second = first + sectors + 1;

            // Generate two triangles per sector
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    this->addMesh({vertices, indices, geom::generateEdges(indices)});
}
