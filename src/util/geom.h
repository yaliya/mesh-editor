#pragma once

#include <vector>
#include "../types//mesh.h"

namespace geom {
    inline glm::vec3 centroid(const std::vector<Vertex>& vertices) {
        // Calculate the centroid
        glm::vec3 centroid(0.0f);

        // Accumulate positions of selected vertices
        for (const auto& vertex : vertices) {
            centroid += vertex.position;
        }

        // Centroid is the sum of vertex positions divided by all given vertices
        return centroid /= static_cast<float>(vertices.size());
    }

    inline glm::vec3 centroid(const std::vector<Vertex*>& vertices) {
        // Calculate the centroid
        glm::vec3 centroid(0.0f);

        // Accumulate positions of selected vertices
        for (const auto& vertex : vertices) {
            centroid += vertex->position;
        }

        // Centroid is the sum of vertex positions divided by all given vertices
        return centroid /= static_cast<float>(vertices.size());
    }

    inline glm::vec3 avgNormal(const std::vector<Vertex*>& vertices) {
        // Calculate average normal
        glm::vec3 averageNormal(0.0f);

        // Accumulate normals of selected vertices
        for (const auto& vertex : vertices) {
            averageNormal += vertex->normal;
        }

        return glm::normalize(averageNormal);
    }

    /**
     *  Calculates the centroid of selected vertices.
     *  Useful when we select multiple vertices and want to translate them together.
     *  Another use case is when we want to reposition the gizmo to the center of the selection
     */
    inline glm::vec3 centroid(const std::vector<std::reference_wrapper<Vertex>>& vertices) {
        // Calculate the centroid
        glm::vec3 centroid(0.0f);

        // Accumulate positions of selected vertices
        for (const auto& vertex : vertices) {
            centroid += vertex.get().position;
        }

        // Centroid is the sum of vertex positions divided by all given vertices
        return centroid /= static_cast<float>(vertices.size());
    }

    /**
     *  Calculates the average normal of selected vertices.
     *  Probably useful when doing translation of multiple selections towards an average normal
     */
    inline glm::vec3 avgNormal(const std::vector<std::reference_wrapper<Vertex>>& vertices) {
        // Calculate average normal
        glm::vec3 averageNormal(0.0f);

        // Accumulate normals of selected vertices
        for (const auto& vertex : vertices) {
            averageNormal += vertex.get().normal;
        }

        return glm::normalize(averageNormal);
    }

    /**
     *  Recalculates normals of the given vertices.
     *  Will try to find indices on each third step (triangle)
     */
    inline void recalculateNormalsForVertices(std::vector<Vertex>& vertices) {
        // Reset all normals for the provided vertices
        for (auto& vertex : vertices) {
            vertex.normal = glm::vec3(0.0f);
        }

        // Iterate through all faces formed by the vertices
        for (size_t i = 0; i < vertices.size(); i += 3) {
            // Ensure we are processing valid triangles
            if (i + 2 >= vertices.size()) {
                break; // Exit if there aren't enough vertices to form a triangle
            }

            // Get references to the three vertices of the triangle
            Vertex& v0 = vertices[i];
            Vertex& v1 = vertices[i + 1];
            Vertex& v2 = vertices[i + 2];

            // Compute the face normal
            glm::vec3 edge1 = v1.position - v0.position;
            glm::vec3 edge2 = v2.position - v0.position;
            glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

            // Accumulate the face normal into each vertex normal
            v0.normal += faceNormal;
            v1.normal += faceNormal;
            v2.normal += faceNormal;
        }

        // Normalize the normals of the vertices
        for (auto& vertex : vertices) {
            vertex.normal = glm::normalize(vertex.normal);
        }
    }

    /**
     * Calculates normals for the entire mesh by iterating over triangles.
     * Useful when we reposition the vertices and want to recalculate the normals to the new position.
     */
    inline void recalculateNormalsForMesh(Mesh& mesh) {
        auto& vertices = mesh.vertices;
        const auto& indices = mesh.indices;

        // Step 1: Reset all vertex normals to zero (for indexed meshes)
        for (auto& vertex : vertices) {
            vertex.normal = glm::vec3(0.0f); // Reset vertex normals to zero
        }

        // Step 2: Iterate through all faces (triangles) using indices
        for (size_t i = 0; i < indices.size(); i += 3) {
            unsigned int index0 = indices[i];
            unsigned int index1 = indices[i + 1];
            unsigned int index2 = indices[i + 2];

            // Access the vertices of the triangle using indices
            Vertex& v0 = vertices[index0];
            Vertex& v1 = vertices[index1];
            Vertex& v2 = vertices[index2];

            // Step 3: Calculate the normal for the face using the cross product of two edges
            glm::vec3 edge1 = v1.position - v0.position;
            glm::vec3 edge2 = v2.position - v0.position;

            // Check if the triangle is degenerate (cross product would be zero)
            glm::vec3 faceNormal = glm::cross(edge1, edge2);

            if (glm::length(faceNormal) == 0.0f) {
                continue; // Skip degenerate triangles
            }

            // faceNormal = glm::normalize(faceNormal); // Normalize the face normal
            //
            // // // Optionally, normalize the normal and weight by area (magnitude of the cross product)
            // float area = glm::length(faceNormal) * 0.5f; // Area is half the magnitude of the cross product

            // Step 4: Add the face normal to each vertex normal
            v0.normal += faceNormal;
            v1.normal += faceNormal;
            v2.normal += faceNormal;
        }

        // Step 5: Normalize all vertex normals after processing all faces
        for (auto& vertex : vertices) {
            if (glm::length(vertex.normal) != 0.0f) {
                vertex.normal = glm::normalize(vertex.normal);
            }
        }
    }

    /**
     * Recalculates the normals on the mesh based on the given vertex index.
     */
    inline void recalculateNormalsForVertex(Mesh& mesh, const unsigned int vertexIndex) {
        glm::vec3 newNormal(0.0f, 0.0f, 0.0f);

        for (unsigned int i = 0; i < mesh.indices.size(); i += 3) {
            unsigned int i0 = mesh.indices[i];
            unsigned int i1 = mesh.indices[i + 1];
            unsigned int i2 = mesh.indices[i + 2];

            // Check if the current vertex is part of the current triangle
            if (i0 == vertexIndex || i1 == vertexIndex || i2 == vertexIndex) {
                const glm::vec3& v0 = mesh.vertices[i0].position;
                const glm::vec3& v1 = mesh.vertices[i1].position;
                const glm::vec3& v2 = mesh.vertices[i2].position;

                // Calculate the edges of the triangle
                glm::vec3 edge1 = v1 - v0;
                glm::vec3 edge2 = v2 - v0;

                // Calculate the face normal using the cross product
                glm::vec3 faceNormal = glm::cross(edge1, edge2);

                // // Optionally, normalize the normal and weight by area (magnitude of the cross product)
                float area = glm::length(faceNormal) * 0.5f; // Area is half the magnitude of the cross product

                // Accumulate the weighted face normal
                newNormal += glm::normalize(faceNormal) * area;
            }
        }

        // Normalize the accumulated normal
        mesh.vertices[vertexIndex].normal = glm::normalize(newNormal);
    }

    /**
     * Calculates the normals of the mesh based on a given position.
     * This will try to find vertices on the mesh that match that position and calculate only those.
     * Useful when we have a portion of vertices that we have translated and want to calculate normals for them.
     */
    inline void recalculateNormalsForAffectedFaces(Mesh& mesh, const glm::vec3& selectedVertexPosition) {
        for (size_t i = 0; i < mesh.indices.size(); i += 3) {
            unsigned int i0 = mesh.indices[i];
            unsigned int i1 = mesh.indices[i + 1];
            unsigned int i2 = mesh.indices[i + 2];

            // Check if the selected vertex's position matches any of the face's vertices
            if (mesh.vertices[i0].position == selectedVertexPosition ||
                mesh.vertices[i1].position == selectedVertexPosition ||
                mesh.vertices[i2].position == selectedVertexPosition) {
                // Recalculate normals for all vertices in the face
                recalculateNormalsForVertex(mesh, i0);
                recalculateNormalsForVertex(mesh, i1);
                recalculateNormalsForVertex(mesh, i2);
            }
        }
    }

    inline std::vector<uint32_t> generateEdges(const std::vector<uint32_t>& indices) {
        std::vector<uint32_t> edges;
        for (size_t i = 0; i < indices.size(); i += 3) {
            // Define edges for each triangle (3 indices per triangle)
            edges.push_back(indices[i]);     // edge from vertex 1 to vertex 2
            edges.push_back(indices[i + 1]);

            edges.push_back(indices[i + 1]); // edge from vertex 2 to vertex 3
            edges.push_back(indices[i + 2]);

            edges.push_back(indices[i + 2]); // edge from vertex 3 to vertex 1
            edges.push_back(indices[i]);
        }

        return edges;
    }
}