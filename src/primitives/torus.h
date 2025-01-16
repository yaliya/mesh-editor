#pragma once

#include "../types/model.h"

class Torus final : public Model {
public:
    Torus(int rings, int sectors, float gap = 1.0f, float tube = 0.4f);
};