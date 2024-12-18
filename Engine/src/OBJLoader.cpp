#include "OBJLoader.hpp"

#include "tiny_obj_loader.h"

#include "glm/gtx/string_cast.hpp"

Mesh OBJLoader::LoadOBJ(const std::string &filePath)
{
    std::vector<SubMesh> subMeshes;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return Mesh("none", subMeshes); // Devuelve un Mesh vacío
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v")
        {
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }
        else if (prefix == "vt")
        {
            glm::vec2 texCoord;
            lineStream >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "vn")
        {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "o" || prefix == "g")
        {
            if (!vertices.empty() || !indices.empty())
            {
                // Crear un SubMesh para el grupo anterior
                SubMesh subMesh;
                subMesh.vertices = vertices;
                subMesh.indices = indices;

                // Configurar el VAO
                subMesh.VAO.Init();
                subMesh.VAO.Bind();
                VBO vbo(vertices);
                EBO ebo(indices);

                subMesh.VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
                subMesh.VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
                subMesh.VAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));

                subMesh.VAO.Unbind();
                vbo.Unbind();
                ebo.Unbind();

                subMeshes.push_back(subMesh);
                vertices.clear();
                indices.clear();
            }
        }
        else if (prefix == "f")
        {
            std::vector<GLuint> vertexIndices;
            std::vector<GLuint> uvIndices;
            std::vector<GLuint> normalIndices;

            std::string vertexData;
            while (lineStream >> vertexData)
            {
                std::istringstream vertexStream(vertexData);
                std::string posIndex, uvIndexStr, normIndexStr;

                // Parse indices formato: v/vt/vn
                std::getline(vertexStream, posIndex, '/');
                std::getline(vertexStream, uvIndexStr, '/');
                std::getline(vertexStream, normIndexStr, '/');

                vertexIndices.push_back(std::stoi(posIndex) - 1);
                uvIndices.push_back(uvIndexStr.empty() ? 0 : std::stoi(uvIndexStr) - 1);
                normalIndices.push_back(normIndexStr.empty() ? 0 : std::stoi(normIndexStr) - 1);
            }

            // Triangular la cara
            for (size_t i = 1; i < vertexIndices.size() - 1; ++i)
            {
                GLuint trianglePosIndices[3] = {
                    vertexIndices[0],
                    vertexIndices[i],
                    vertexIndices[i + 1]};

                GLuint triangleUVIndices[3] = {
                    uvIndices[0],
                    uvIndices[i],
                    uvIndices[i + 1]};

                GLuint triangleNormalIndices[3] = {
                    normalIndices[0],
                    normalIndices[i],
                    normalIndices[i + 1]};

                for (int j = 0; j < 3; ++j)
                {
                    Vertex vertex = {};
                    vertex.position = tempPositions[trianglePosIndices[j]];
                    vertex.texUV = triangleUVIndices[j] < tempTexCoords.size() ? tempTexCoords[triangleUVIndices[j]] : glm::vec2(0.0f, 0.0f);
                    vertex.normal = triangleNormalIndices[j] < tempNormals.size() ? tempNormals[triangleNormalIndices[j]] : glm::vec3(0.0f, 0.0f, 0.0f);

                    vertices.push_back(vertex);
                    indices.push_back(static_cast<GLuint>(vertices.size() - 1));
                }
            }
        }
    }

    // Añadir el último submesh
    if (!vertices.empty() || !indices.empty())
    {
        SubMesh subMesh;
        subMesh.vertices = vertices;
        subMesh.indices = indices;

        subMesh.VAO.Init();
        subMesh.VAO.Bind();
        VBO vbo(vertices);
        EBO ebo(indices);

        subMesh.VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
        subMesh.VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
        subMesh.VAO.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));

        subMesh.VAO.Unbind();
        vbo.Unbind();
        ebo.Unbind();

        subMeshes.push_back(subMesh);
    }

    file.close();
    for (int i = 0; i < subMeshes.size(); i++)
    {
        std::cout << "SubMesh " << i << " vertices: " << subMeshes[i].vertices.size() << " indices: " << subMeshes[i].indices.size() << std::endl;
    }
    return Mesh("EEEEEONO", subMeshes); // Devuelve el objeto Mesh construido
}
