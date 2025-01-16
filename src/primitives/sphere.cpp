#include "sphere.h"
#include "../types/mesh.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../util/geom.h"

Sphere::Sphere(int latitude, int longitude, float radius) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> edges;

    // Generate vertices
    for (int lat = 0; lat <= latitude; ++lat) {
        float theta = lat * glm::pi<float>() / latitude; // Adjusted to latitude
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (int lon = 0; lon <= longitude; ++lon) {
            float phi = lon * 2 * glm::pi<float>() / longitude;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            glm::vec3 position(
                radius * cosPhi * sinTheta,
                radius * cosTheta,
                radius * sinPhi * sinTheta
            );

            glm::vec3 normal = glm::normalize(position);

            vertices.push_back(Vertex(position, normal));
        }
    }

    // Generate indices for the triangles
    for (int lat = 0; lat < latitude; ++lat) {
        for (int lon = 0; lon < longitude; ++lon) {
            int first = (lat * (longitude + 1)) + lon;
            int second = first + longitude + 1;

            // First triangle (use counter-clockwise order)
            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            // Second triangle (use counter-clockwise order)
            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second + 1);
        }
    }

    // // Generate edges
    // for (int lat = 0; lat <= latitude; ++lat) {
    //     for (int lon = 0; lon <= longitude; ++lon) {
    //         // Connect to the next longitude (right)
    //         if (lon < longitude) {
    //             edges.push_back(lat * (longitude + 1) + lon);
    //             edges.push_back(lat * (longitude + 1) + lon + 1);
    //         }
    //         // Connect to the next latitude (down)
    //         if (lat < latitude) {
    //             edges.push_back(lat * (longitude + 1) + lon);
    //             edges.push_back((lat + 1) * (longitude + 1) + lon);
    //         }
    //     }
    // }

    this->addMesh({vertices, indices, geom::generateEdges(indices)});
}
