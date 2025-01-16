#pragma once

#include "mesh.h"

class Model {
    std::vector<Mesh> meshes;
public:
    Model();
    virtual ~Model();
    explicit Model(const std::vector<Mesh> &meshes);

    void addMesh(const Mesh& mesh);
    std::vector<Mesh>& getMeshes();
};
