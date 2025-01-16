#pragma once

#include <glad/glad.h>
#include <vector>

#include "../../util/singleton.h"
#include "../../types/vertex.h"

class EdgeSelectionState : Singleton {
    // Array of vertex pairs that make up an edge
    std::vector<std::pair<Vertex*, Vertex*>> edges;

public:
    static void add(const std::pair<Vertex*, Vertex*>& edge) {
        instance<EdgeSelectionState>().edges.emplace_back(edge);
    }

    static std::vector<std::pair<Vertex*, Vertex*>>& get() {
        return instance<EdgeSelectionState>().edges;
    }

    static void pop() {
        if (!instance<EdgeSelectionState>().edges.empty()) {
            instance<EdgeSelectionState>().edges.pop_back();
        }
    }

    static void clear() {
        instance<EdgeSelectionState>().edges.clear();
    }

    static void draw(const std::pair<Vertex*, Vertex*>& edge) {
        GLuint vao, vbo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Create a vector to hold the positions of the two vertices of the selected edge
        const std::vector<glm::vec3> positions = {edge.first->position, edge.second->position};

        // Bind the new data to the VAO and VBO
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);

        // Set vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindVertexArray(0); // Unbind the VAO to prevent unintended changes

        glDisable(GL_DEPTH_TEST);
        glLineWidth(3.0f);

        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        glLineWidth(1.0f);
        glEnable(GL_DEPTH_TEST);
    }
};
