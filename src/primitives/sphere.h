#pragma once

#include "../types/model.h"

class Sphere final : public Model {
public:
    Sphere(int latitude, int longitude, float radius);
};