#include "OBJLoader.hpp"

#include "tiny_obj_loader.h"

#include "glm/gtx/string_cast.hpp"

Mesh OBJLoader::LoadOBJWithTinyOBJ(const std::string &filePath)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures; // Optional, not loaded here but can be extended.

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str()))
    {
        std::cerr << warn << std::endl;
        std::cerr << err << std::endl;
        return Mesh(vertices, indices, textures);
    }

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex = {};
            vertex.position = {attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1], attrib.vertices[3 * index.vertex_index + 2]};
            vertex.texUV = {attrib.texcoords[2 * index.texcoord_index + 0], 1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
            vertex.normal = {attrib.normals[3 * index.normal_index + 0], attrib.normals[3 * index.normal_index + 1], attrib.normals[3 * index.normal_index + 2]};
            vertices.push_back(vertex);
            indices.push_back(static_cast<GLuint>(indices.size()));
        }
    }

    printf("Loaded mesh with %lu vertices and %lu indices\n", vertices.size(), indices.size());
    return Mesh(vertices, indices, textures);
}

Mesh OBJLoader::LoadOBJ(const std::string &filePath)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures; // Optional, not loaded here but can be extended.

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    int numVertices = 0;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return Mesh(vertices, indices, textures);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v")
        { // Vertex positions
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        }
        else if (prefix == "vt")
        { // Texture coordinates
            glm::vec2 texCoord;
            lineStream >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "vn")
        { // Normals
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "f")
        {
            std::vector<GLuint> vertexIndices;
            std::vector<GLuint> uvIndices;
            std::vector<GLuint> normalIndices;

            std::string vertexData;
            while (lineStream >> vertexData) // Lee todos los vértices de la cara
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

            // Trianguliza la cara
            for (size_t i = 1; i < vertexIndices.size() - 1; ++i)
            {
                // Vértices del triángulo
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

                // Añade cada triángulo como tres vértices
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

        else if (prefix == "mtllib")
        {
            std::string mtlPath;
            lineStream >> mtlPath;
            std::cout << "MTL file: " << mtlPath << std::endl;
        }
        else if (prefix == "holi")
        {
            std::cout << "Holi" << std::endl;
        }
    }
    printf("Loaded mesh with %lu vertices and %lu indices\n", vertices.size(), indices.size());
    printf("Loaded mesh with %lu positions, %lu normals, and %lu texCoords\n", tempPositions.size(), tempNormals.size(), tempTexCoords.size());
    printf("Loaded mesh with %d vertices2\n", numVertices);
    file.close();
    return Mesh(vertices, indices, textures);
}
