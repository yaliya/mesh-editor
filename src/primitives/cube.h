#pragma once

#include "../types/model.h"

class Cube final : public Model {
public:
    explicit Cube(float sideLength, int subdivisions);
};