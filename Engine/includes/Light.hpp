#pragma once

#include <string>

#define GL_SILENCE_DEPRECATION

#include "Camera.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"
#include "Object.hpp"

class Light : public Object
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Texture texture; // Textura para visualizar la posición de la luz
    VAO VAO;

    enum LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };
    LightType lightType; // Tipo de luz

    glm::vec3 color; // Color de la luz
    float intensity; // Intensidad de la luz
    float range;     // Rango de la luz (para luces puntuales)

    // Constructor de la luz
    Light(const std::string &name, LightType type, const glm::vec3 &color, float intensity);

    // // Dibujar la luz
    void Draw(
        Shader &shader,
        Camera &camera) override;
};
