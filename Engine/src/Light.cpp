#include "Light.hpp"

#include <filesystem>
namespace fs = std::filesystem;

Light::Light(const std::string &name, LightType type, const glm::vec3 &color, float intensity)
    : Object(name), lightType(type), color(color), intensity(intensity), range(10.0f)
{
    fs::path path = fs::current_path();
    std::string texturePath = path.string() + "/assets/textures/lightIcon.png";
    this->texture = Texture(texturePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    Vertex vertex;
    vertex.position = {-1.5f, -1.5f, 1.5f};
    vertex.normal = {0.0f, 0.0f, 1.0f};
    vertex.texUV = {0.0f, 0.0f};
    vertices.push_back(vertex);
    vertex.position = {1.5f, -1.5f, 1.5f};
    vertex.normal = {0.0f, 0.0f, 1.0f};
    vertex.texUV = {1.0f, 0.0f};
    vertices.push_back(vertex);
    vertex.position = {1.5f, 1.5f, 1.5f};
    vertex.normal = {0.0f, 0.0f, 1.0f};
    vertex.texUV = {1.0f, 1.0f};
    vertices.push_back(vertex);
    vertex.position = {-1.5f, 1.5f, 1.5f};
    vertex.normal = {0.0f, 0.0f, 1.0f};
    vertex.texUV = {0.0f, 1.0f};
    vertices.push_back(vertex);
    indices = {0, 1, 2, 2, 3, 0};

    VAO.Init();
    VAO.Bind();
    VBO vbo(vertices);
    EBO ebo(indices);
    VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
    VAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));
    VAO.Unbind();
    vbo.Unbind();
    ebo.Unbind();
    this->type = "Light";
}

void Light::Draw(
    Shader &shader,
    Camera &camera)
{
    shader.Activate();
    this->texture.texUnit(shader, "tex0", 0);
    this->texture.Bind();
    this->VAO.Bind();
    camera.Matrix(shader, "camMatrix");

    glm::mat4 matrix = glm::mat4(1.0f);

    glm::vec3 cameraDirection = glm::normalize(camera.Position - this->position);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(up, cameraDirection));
    up = glm::cross(cameraDirection, right);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix[0] = glm::vec4(right, 0.0f);
    rotationMatrix[1] = glm::vec4(up, 0.0f);
    rotationMatrix[2] = glm::vec4(cameraDirection, 0.0f);

    matrix = glm::translate(matrix, this->position);
    matrix *= rotationMatrix;
    // matrix = glm::scale(matrix, this->scale);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
    glUniform3fv(glGetUniformLocation(shader.ID, "cameraPos"), 1, glm::value_ptr(camera.Position));

    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}