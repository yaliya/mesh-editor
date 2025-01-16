#pragma once

#include <glad/glad.h>
#include <vector>

#include "../../types/vertex.h"
#include "../../util/singleton.h"

class FaceSelectionState : Singleton {
    std::vector<std::vector<Vertex*>> faces;

public:
    static void add(const std::vector<Vertex*>& face) {
        instance<FaceSelectionState>().faces.emplace_back(face);
    }

    static std::vector<std::vector<Vertex*>> get() {
        return instance<FaceSelectionState>().faces;
    }

    static void pop() {
        if (!instance<FaceSelectionState>().faces.empty()) {
             instance<FaceSelectionState>().faces.pop_back();
        }
    }

    static void clear() {
        instance<FaceSelectionState>().faces.clear();
    }

    static void draw(const std::vector<Vertex*>& face) {
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        std::vector<glm::vec3> facePositions;
        facePositions.reserve(3);

        for (const auto& vertex : face) {
            facePositions.push_back(vertex->position);
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, facePositions.size() * sizeof(glm::vec3), facePositions.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindVertexArray(0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};
