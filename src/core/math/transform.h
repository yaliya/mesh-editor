#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 rotation = glm::vec3(0, 0, 0);
    glm::vec3 scaling = glm::vec3(1, 1, 1);
    glm::mat4 transform = glm::mat4(1.0f);

    void translate(const glm::vec3& position) {
        this->position = position;
        this->transform = this->getTransform();
    }

    void rotate(const glm::vec3& rotation) {
        this->rotation = rotation;
        this->transform = this->getTransform();
    }

    void scale(const glm::vec3& scale) {
        this->scaling = scale;
        this->transform = this->getTransform();
    }

    [[nodiscard]] glm::mat4 getTransform() const {
        glm::mat4 mat = glm::mat4(1.0f);
        mat = glm::translate(mat, position);               // Translation
        mat *= glm::toMat4(glm::quat(rotation));                   // Rotation
        mat = glm::scale(mat, scaling);                      // Scaling
        return mat;
    }
};
