#pragma once

#include "../types/model.h"

class Cone final : public Model {
public:
    Cone(float radius, float height, int segments, int verticalDivisions);
};