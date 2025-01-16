#pragma once

#include <glad/glad.h>
#include "../../types/vertex.h"

class VertexSelectionState final : Singleton {
    std::vector<Vertex*> vertices;

public:
    static void add(Vertex* vertex) {
        instance<VertexSelectionState>().vertices.emplace_back(vertex);
    }

    static std::vector<Vertex*>& get() {
        return instance<VertexSelectionState>().vertices;
    }

    static void pop() {
        instance<VertexSelectionState>().vertices.pop_back();
    }

    static void clear() {
        instance<VertexSelectionState>().vertices.clear();
    }

    static void draw(const Vertex* vertex, const glm::mat4& transform) {
        unsigned int vao, vbo;

        glDisable(GL_DEPTH_TEST);
        glPointSize(10.0f);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glm::vec4 transformedPos = transform * glm::vec4(vertex->position, 1.0f);  // Apply transformation

        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &transformedPos, GL_STATIC_DRAW);

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), vertex, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);  // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

        glEnableVertexAttribArray(1);  // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
        glBindVertexArray(0);

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
};
