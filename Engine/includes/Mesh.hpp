#pragma once

#include <string>

#include "Camera.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"
struct SubMesh
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Texture texture; // Cada sub-mesh tiene una textura asociada
    VAO VAO;         // Cada sub-mesh tiene su propio VAO
};

class Mesh
{
public:
    std::vector<SubMesh> subMeshes;

    // Constructor
    Mesh(std::vector<SubMesh> &subMeshes);
    Mesh()
    {
    }

    // Draws the mesh
    void Draw(
        Shader &shader,
        Camera &camera,
        glm::mat4 matrix = glm::mat4(1.0f),
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};