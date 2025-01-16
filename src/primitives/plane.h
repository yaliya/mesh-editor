#pragma once

#include "../types/model.h"

class Plane final : public Model {
public:
    Plane(float width, float depth, unsigned int subdivisionsX, unsigned int subdivisionsZ);
};