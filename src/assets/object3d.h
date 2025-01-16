#pragma once

#include <string>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "asset.h"
#include "../types/mesh.h"

class Object3D : public Asset {
    std::string directory;
    std::vector<Mesh> meshes;

    void processNode(const aiNode *node, const aiScene *scene, const glm::mat4& parentTransform);
    Mesh processMesh(const aiMesh * mesh, const aiScene * scene, const glm::mat4& transform) const;
public:
    explicit Object3D(const std::string& path);
    ~Object3D() override;

    bool load() override;
    std::vector<Mesh>& getMeshes();
    std::vector<std::shared_ptr<Texture>> loadTextures(const aiMaterial * material, aiTextureType type) const;
};
