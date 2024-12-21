#include "Mesh.hpp"
#include <filesystem>
namespace fs = std::filesystem;

void SubMesh::Draw(Shader &shader, Camera &camera,
                   glm::vec3 mainPosition, glm::vec3 mainRotation, glm::vec3 mainScale)
{
    shader.Activate();
    int isTextured = glGetUniformLocation(shader.ID, "isTextured");
    if (this->texture.ID != -1)
    {
        this->texture.texUnit(shader, "tex0", 0);
        glUniform1i(isTextured, 1);
        this->texture.Bind();
    }
    else
    {
        glUniform1i(isTextured, 0);
    }
    this->VAO.Bind();
    camera.Matrix(shader, "camMatrix");
    glm::mat4 matrix = glm::mat4(1.0f);

    glm::quat rot = glm::quat(this->rotation);
    matrix = glm::translate(matrix, this->position);
    matrix *= glm::mat4_cast(rot);
    matrix = glm::scale(matrix, this->scale);

    // Añadir la transformación del objeto principal
    matrix = glm::translate(matrix, mainPosition);
    matrix = glm::scale(matrix, mainScale);
    matrix = glm::rotate(matrix, glm::radians(mainRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(mainRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(mainRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));

    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh::Mesh(const std::string &name, const std::vector<SubMesh> &tempSubMeshes) : Object(name)
{
    this->type = "Mesh";
    Mesh::subMeshes = tempSubMeshes;
    for (int i = 0; i < subMeshes.size(); i++)
    {
        subMeshes[i].VAO.Init();
        subMeshes[i].VAO.Bind();
        VBO vbo(subMeshes[i].vertices);
        EBO ebo(subMeshes[i].indices);
        subMeshes[i].VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
        subMeshes[i].VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
        subMeshes[i].VAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));
        subMeshes[i].VAO.Unbind();
        vbo.Unbind();
        ebo.Unbind();
    }
}

void Mesh::Draw(
    Shader &shader,
    Camera &camera)
{
    // // Bind shader to be able to access uniforms

    for (int i = 0; i < subMeshes.size(); i++)
    {
        subMeshes[i].Draw(shader, camera, this->position, this->rotation, this->scale);
    }
}