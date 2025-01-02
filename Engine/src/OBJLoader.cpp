#include "OBJLoader.hpp"

#include "tiny_obj_loader.h"

#include "glm/gtx/string_cast.hpp"

struct MTL
{
    std::string name;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Ns;
    float d;
    std::string map_Kd;
};

std::vector<MTL> LoadMTL(const std::string &filePath)
{
    std::vector<MTL> materials;
    MTL currentMaterial;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error al abrir el archivo MTL: " << filePath << std::endl;
        return materials;
    }
    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;
        if (prefix == "newmtl")
        {
            if (!currentMaterial.name.empty())
            {
                materials.push_back(currentMaterial);
            }
            currentMaterial = {};
            lineStream >> currentMaterial.name;
        }
        else if (prefix == "Ka")
        {
            lineStream >> currentMaterial.Ka.r >> currentMaterial.Ka.g >> currentMaterial.Ka.b;
        }
        else if (prefix == "Kd")
        {
            lineStream >> currentMaterial.Kd.r >> currentMaterial.Kd.g >> currentMaterial.Kd.b;
        }
        else if (prefix == "Ks")
        {
            lineStream >> currentMaterial.Ks.r >> currentMaterial.Ks.g >> currentMaterial.Ks.b;
        }
        else if (prefix == "Ns")
        {
            lineStream >> currentMaterial.Ns;
        }
        else if (prefix == "d")
        {
            lineStream >> currentMaterial.d;
        }
        else if (prefix == "map_Kd")
        {
            lineStream >> currentMaterial.map_Kd;
        }
    }
    materials.push_back(currentMaterial);
    file.close();
    return materials;
}

Mesh OBJLoader::LoadOBJ(const std::string &currentPath, const std::string &fileName)
{
    std::vector<SubMesh> subMeshes;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::string filePath = currentPath + "/" + fileName;

    std::string tempName;

    std::ifstream file(filePath);
    std::vector<MTL> mtl = LoadMTL(filePath.substr(0, filePath.size() - 4) + ".mtl");
    MTL currentMaterial;

    std::map<std::string, Texture> textures;
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
            normal = glm::normalize(normal);
            tempNormals.push_back(normal);
        }
        else if (prefix == "usemtl")
        {
            std::string materialName;
            lineStream >> materialName;
            for (const auto &material : mtl)
            {
                if (material.name == materialName)
                    currentMaterial = material;
            }
        }
        else if (prefix == "o" || prefix == "g")
        {
            if (!vertices.empty() || !indices.empty())
            {
                if (tempName.empty())
                    tempName = "default";
                SubMesh subMesh;
                subMesh.name = tempName;
                subMesh.vertices = vertices;
                subMesh.indices = indices;

                lineStream >> tempName;
                subMeshes.push_back(subMesh);
                vertices.clear();
                indices.clear();
            }
            else
            {
                lineStream >> tempName;
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
                    vertex.color = currentMaterial.Kd;

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
        if (tempName.empty())
            tempName = "empty";
        else
            subMesh.name = tempName;

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
    return Mesh("empty", subMeshes); // Devuelve el objeto Mesh construido
}
