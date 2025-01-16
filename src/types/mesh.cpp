#include "mesh.h"

#include <numeric>
#include <glad/glad.h>

#include "../util/geom.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    this->vertices = vertices;
    this->indices = indices;

    this->bind();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges) {
    this->vertices = vertices;
    this->indices = indices;
    this->edges = edges;

    this->bind();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<std::shared_ptr<Texture>>& textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->bind();
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<uint32_t> &edges, const std::vector<std::shared_ptr<Texture> > &textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->edges = edges;
    this->textures = textures;

    this->bind();
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<uint32_t> &edges, const std::vector<std::shared_ptr<Texture> > &textures, const glm::mat4& transform) {
    this->vertices = vertices;
    this->indices = indices;
    this->edges = edges;
    this->textures = textures;
    this->transform.transform = transform;

    this->bind();
}

void Mesh::bind() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_DYNAMIC_DRAW);

    // Indices buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_DYNAMIC_DRAW);

    // Vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Setting up the edges VAO, VBO, and EBO
    glGenVertexArrays(1, &edgeVao);
    glBindVertexArray(edgeVao);
    glGenBuffers(1, &edgeVbo);
    glGenBuffers(1, &edgeEbo);

    // Bind and set up the VBO for edges
    glBindBuffer(GL_ARRAY_BUFFER, edgeVbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_DYNAMIC_DRAW);

    // Indices buffer for edges
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edgeEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->edges.size() * sizeof(GLuint), &this->edges[0], GL_DYNAMIC_DRAW);

    // Vertex attributes for edges
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void *>(nullptr));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}