#include "object3d.h"

#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../managers/asset_manager.h"
#include "../util/geom.h"

Object3D::Object3D(const std::string& path) : Asset(path) {}

Object3D::~Object3D() = default;

bool Object3D::load() {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
        aiProcess_Triangulate
        | aiProcess_FlipUVs
        | aiProcess_OptimizeMeshes
        | aiProcess_JoinIdenticalVertices
        | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return false;
    }

    directory = path.substr(0, path.find_last_of('/'));

    auto aiMat4ToGlm = [](const aiMatrix4x4& aiMat) -> glm::mat4 {
        glm::mat4 mat;
        mat[0][0] = aiMat.a1; mat[1][0] = aiMat.a2; mat[2][0] = aiMat.a3; mat[3][0] = aiMat.a4;
        mat[0][1] = aiMat.b1; mat[1][1] = aiMat.b2; mat[2][1] = aiMat.b3; mat[3][1] = aiMat.b4;
        mat[0][2] = aiMat.c1; mat[1][2] = aiMat.c2; mat[2][2] = aiMat.c3; mat[3][2] = aiMat.c4;
        mat[0][3] = aiMat.d1; mat[1][3] = aiMat.d2; mat[2][3] = aiMat.d3; mat[3][3] = aiMat.d4;
        return mat;
    };

    processNode(scene->mRootNode, scene, aiMat4ToGlm(scene->mRootNode->mTransformation));

    return true;
}

void Object3D::processNode(const aiNode *node, const aiScene *scene, const glm::mat4& parentTransform) {
    meshes.reserve(node->mNumMeshes);

    auto aiMat4ToGlm = [](const aiMatrix4x4& aiMat) -> glm::mat4 {
        glm::mat4 mat;
        mat[0][0] = aiMat.a1; mat[1][0] = aiMat.a2; mat[2][0] = aiMat.a3; mat[3][0] = aiMat.a4;
        mat[0][1] = aiMat.b1; mat[1][1] = aiMat.b2; mat[2][1] = aiMat.b3; mat[3][1] = aiMat.b4;
        mat[0][2] = aiMat.c1; mat[1][2] = aiMat.c2; mat[2][2] = aiMat.c3; mat[3][2] = aiMat.c4;
        mat[0][3] = aiMat.d1; mat[1][3] = aiMat.d2; mat[2][3] = aiMat.d3; mat[3][3] = aiMat.d4;
        return mat;
    };

    glm::mat4 nodeTransform = parentTransform * aiMat4ToGlm(node->mTransformation);

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene, nodeTransform));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, nodeTransform);
    }
}

Mesh Object3D::processMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4& transform) const {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    vertices.reserve(mesh->mNumVertices);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.emplace_back(
            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
            glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
            glm::vec2(mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y)
        );
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const auto& face = mesh->mFaces[i];
        indices.reserve(face.mNumIndices);

        for (unsigned j = 0; j < face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<std::shared_ptr<Texture>> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
    std::vector<std::shared_ptr<Texture>> specularMaps = loadTextures(material, aiTextureType_SPECULAR);

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // Extract unique edges
    auto edges = geom::generateEdges(indices);

    return {vertices, indices, edges, textures, transform};
}

std::vector<std::shared_ptr<Texture>> Object3D::loadTextures(const aiMaterial * material, const aiTextureType type) const {
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;

        material->GetTexture(type, i, &str);
        TextureType textureType;

        if (type == aiTextureType_DIFFUSE) {
            textureType = TextureType::DIFFUSE_MAP;
        }

        else if (type == aiTextureType_SPECULAR) {
            textureType = TextureType::SPECULAR_MAP;
        }

        auto texture = AssetManager::load<Texture>(directory + "/" + str.C_Str(), textureType);

        if (texture != nullptr) {
            textures.push_back(texture);
        }
    }

    return textures;
}

std::vector<Mesh> &Object3D::getMeshes() {
    return meshes;
}
