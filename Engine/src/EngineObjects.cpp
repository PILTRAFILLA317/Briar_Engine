#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Engine::AddOBJModel(const std::string &name, const std::string &path)
{
    Mesh mesh = OBJLoader::LoadOBJ(path, name);
    mesh.name = name;
    AddObject(std::make_shared<Mesh>(mesh));
}

void Engine::CreatePlane()
{
    std::vector<Vertex> vertices = {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}};

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3};

    std::vector<SubMesh> subMeshes;

    SubMesh subMesh;
    subMesh.vertices = vertices;
    subMesh.indices = indices;

    subMesh.VAO.Init();
    subMesh.VAO.Bind();
    VBO vbo(vertices);
    EBO ebo(indices);

    subMesh.VAO.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    subMesh.VAO.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
    subMesh.VAO.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));

    subMesh.VAO.Unbind();
    vbo.Unbind();
    ebo.Unbind();

    subMeshes.push_back(subMesh);

    auto plane = std::make_shared<Mesh>("Plane", subMeshes);
    AddObject(plane);
}