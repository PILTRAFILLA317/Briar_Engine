#include "Mesh.hpp"
#include <filesystem>
namespace fs = std::filesystem;

// Mesh::Mesh(std::vector<SubMesh> &subMeshes)
// {
//     Mesh::subMeshes = subMeshes;
// }

void Mesh::Draw(
    Shader &shader,
    Camera &camera)
{
    // // Bind shader to be able to access uniforms

    for (int i = 0; i < subMeshes.size(); i++)
    {
        shader.Activate();
        subMeshes[i].VAO.Bind();
        subMeshes[i].texture.Bind();
        camera.Matrix(shader, "camMatrix");
        glDrawElements(GL_TRIANGLES, subMeshes[i].indices.size(), GL_UNSIGNED_INT, 0);
    }
}