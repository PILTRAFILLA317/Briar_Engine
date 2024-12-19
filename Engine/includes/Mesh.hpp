#pragma once

#include <string>

#include "Camera.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"
#include "Object.hpp"

class SubMesh : public Object
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    // std::string name;
    std::string texturePath;
    Texture texture; // Cada sub-mesh tiene una textura asociada
    VAO VAO;

    SubMesh(const std::string &name = "Default") : Object(name) {
        this->type = "SubMesh";
    }

    void Draw(Shader &shader, Camera &camera) override;
};

class Mesh : public Object
{
public:
    std::vector<SubMesh> subMeshes;

    // Constructor
    Mesh(const std::string &name, const std::vector<SubMesh> &subMeshes);

    Mesh(const std::string &name = "Unnamed")
        : Object(name)
    {
        this->type = "Mesh";
    }

    // Draws the mesh
    void Draw(
        Shader &shader,
        Camera &camera) override;
};