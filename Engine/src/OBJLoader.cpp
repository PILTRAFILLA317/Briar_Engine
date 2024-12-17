#include "OBJLoader.hpp"

#include "glm/gtx/string_cast.hpp"

Mesh OBJLoader::LoadOBJ(const std::string& filePath) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures; // Optional, not loaded here but can be extended.

    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return Mesh(vertices, indices, textures);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v") { // Vertex positions
            glm::vec3 position;
            lineStream >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        } else if (prefix == "vt") { // Texture coordinates
            glm::vec2 texCoord;
            lineStream >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        } else if (prefix == "vn") { // Normals
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (prefix == "f") { // Faces
            std::string vertexData;
            GLuint vertexIndex[3], uvIndex[3], normalIndex[3];

            for (int i = 0; i < 3; ++i) {
                lineStream >> vertexData;

                std::istringstream vertexStream(vertexData);
                std::string posIndex, uvIndexStr, normIndexStr;

                // Parse indices format: v/vt/vn
                std::getline(vertexStream, posIndex, '/');
                std::getline(vertexStream, uvIndexStr, '/');
                std::getline(vertexStream, normIndexStr, '/');

                vertexIndex[i] = std::stoi(posIndex) - 1;
                uvIndex[i] = uvIndexStr.empty() ? 0 : std::stoi(uvIndexStr) - 1;
                normalIndex[i] = normIndexStr.empty() ? 0 : std::stoi(normIndexStr) - 1;
            }

            for (int i = 0; i < 3; ++i) {
                Vertex vertex = {};
                vertex.position = tempPositions[vertexIndex[i]];
                vertex.texUV = uvIndex[i] < tempTexCoords.size() ? tempTexCoords[uvIndex[i]] : glm::vec2(0.0f, 0.0f);
                vertex.normal = normalIndex[i] < tempNormals.size() ? tempNormals[normalIndex[i]] : glm::vec3(0.0f, 0.0f, 0.0f);

                // Add the vertex and index
                vertices.push_back(vertex);
                indices.push_back(static_cast<GLuint>(vertices.size() - 1));
            }
        }
    }
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << "POSITION "<< glm::to_string(vertices[i].position) << std::endl;
        std::cout << "NORMAL "<< glm::to_string(vertices[i].normal) << std::endl;
        std::cout << "TEXCOORDS "<< glm::to_string(vertices[i].texUV) << std::endl;
        std::cout << "COLOR "<< glm::to_string(vertices[i].color) << std::endl;
        std::cout << "-------------------" << std::endl;
    }
    file.close();
    return Mesh(vertices, indices, textures);
}
