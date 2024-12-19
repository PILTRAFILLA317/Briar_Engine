#include "Mesh.hpp"
#include <filesystem>
namespace fs = std::filesystem;

void SubMesh::Draw(Shader &shader, Camera &camera)
{
    int isTextured = glGetUniformLocation(shader.ID, "isTextured");
    shader.Activate();
    if (this->texturePath != "")
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
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh::Mesh(const std::string &name, const std::vector<SubMesh> &tempSubMeshes) : Object(name)
{
    this->type = "Mesh";
    Mesh::subMeshes = tempSubMeshes;
    for (int i = 0; i < subMeshes.size(); i++)
    {
        // subMeshes[i].VAO.Init();
        // subMeshes[i].VAO.Bind();
        // VBO vbo(subMeshes[i].vertices);
        // EBO ebo(subMeshes[i].indices);
        // subMeshes[i].VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
        // subMeshes[i].VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
        // subMeshes[i].VAO.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
        // subMeshes[i].VAO.Unbind();
        // vbo.Unbind();
        // ebo.Unbind();
        if (subMeshes[i].texturePath != "")
        {
            Texture tempTexture = Texture(subMeshes[i].texturePath.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
            subMeshes[i].texture = tempTexture;
        }
    }
}

void Mesh::Draw(
    Shader &shader,
    Camera &camera)
{
    // // Bind shader to be able to access uniforms

    for (int i = 0; i < subMeshes.size(); i++)
    {
        subMeshes[i].Draw(shader, camera);
    }
}