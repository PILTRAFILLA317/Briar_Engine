#pragma once

#include <string>
#include <glm/glm.hpp>

class Object
{
public:
    std::string type = "EmptyObject";  // Tipo de objeto (por defecto "Object")
    std::string name;                  // Nombre del objeto (opcional)
    glm::vec3 position;                // Posición en el espacio
    glm::vec3 rotation;                // Rotación (como Euler o Quaternion)
    glm::vec3 scale = glm::vec3(1.0f); // Escala (por defecto 1.0)

    Object(const std::string &name = "Unnamed")
        : name(name), position(0.0f), rotation(0.0f), scale(1.0f) {}

    virtual void Update(float deltaTime) {} // Lógica de actualización específica del objeto
    virtual void Draw(
        Shader &shader,
        Camera &camera) {} // Método virtual para dibujar
    virtual ~Object() {}
};
