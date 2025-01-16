#pragma once

#include <memory>
#include <vector>
#include "vertex.h"
#include "../assets/texture.h"
#include "../core/math/transform.h"

struct Mesh {
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int ebo{};

    unsigned int edgeVao{};
    unsigned int edgeVbo{};
    unsigned int edgeEbo{};

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<uint32_t> edges;
    std::vector<std::shared_ptr<Texture>> textures;

    Transform transform;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges, const std::vector<std::shared_ptr<Texture>>& textures);
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<uint32_t>& edges, const std::vector<std::shared_ptr<Texture>>& textures, const glm::mat4& transform);

    void bind();
};
