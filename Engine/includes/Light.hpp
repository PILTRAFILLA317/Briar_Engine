#pragma once

#include "Object.hpp"

class Light : public Object {
public:
    enum LightType { DIRECTIONAL, POINT, SPOT };
    LightType type;          // Tipo de luz

    glm::vec3 color;         // Color de la luz
    float intensity;         // Intensidad de la luz
    float range;             // Rango de la luz (para luces puntuales)

    // Constructor para luces
    Light(const std::string& name, LightType type, glm::vec3 color, float intensity)
        : Object(name), type(type), color(color), intensity(intensity), range(10.0f) {}
};
