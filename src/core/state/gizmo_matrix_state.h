#pragma once

#include <glm/glm.hpp>
#include "../../util/singleton.h"

class GizmoMatrixState : Singleton {
    glm::mat4 model = glm::mat4(1.0f);
public:
    static glm::mat4& get() {
        return instance<GizmoMatrixState>().model;
    }

    static void set(const glm::mat4& model) {
        instance<GizmoMatrixState>().model = model;
    }

    static void clear() {
        instance<GizmoMatrixState>().model = glm::mat4(1.0f);
    }
};
