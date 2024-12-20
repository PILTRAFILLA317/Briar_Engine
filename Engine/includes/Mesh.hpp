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
    // std::string texturePath;
    Texture texture; // Cada sub-mesh tiene una textura asociada
    VAO VAO;

    SubMesh(const std::string &name = "Default") : Object(name)
    {
        this->type = "SubMesh";
    }

    void UpdateTexture(const std::string &texturePath)
    {
        // this->texturePath = texturePath;
        this->texture = Texture(texturePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    }

    // void Update() override;

    void Draw(
        Shader &shader,
        Camera &camera,
        glm::vec3 position,
        glm::vec3 rotation,
        glm::vec3 scale);
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