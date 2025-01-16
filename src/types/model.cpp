#include "model.h"

Model::Model() = default;
Model::~Model() = default;

Model::Model(const std::vector<Mesh> &meshes) {
    this->meshes = meshes;
}

void Model::addMesh(const Mesh& mesh) {
    this->meshes.push_back(mesh);
}

std::vector<Mesh> &Model::getMeshes() {
    return this->meshes;
}