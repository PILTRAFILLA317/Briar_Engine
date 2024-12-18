#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#define GL_SILENCE_DEPRECATION
#include "FrameBuffer.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <stb_image.h>
#include <string>

#include "Mesh.hpp"
#include "Object.hpp"
#include "OBJLoader.hpp"

struct color
{
    GLfloat r, g, b, a;
};

class Engine
{
public:
    Engine(int width, int height, const std::string &title)
        : windowWidth(width), windowHeight(height), windowTitle(title) {}

    void Init();

    void Run();

    ~Engine()
    {
        // Limpieza
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        shaderProgram.Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
        delete camera;
        delete framebuffer;
    }

private:
    std::string basePath;
    int windowWidth, windowHeight;
    std::string windowTitle;
    GLFWwindow *window = nullptr;
    Camera *camera = nullptr;
    FrameBuffer *framebuffer = nullptr;
    color clearColor = {0.1f, 0.1f, 0.1f, 1.0f};
    float deltaTime = 0.0f, lastFrame = 0.0f;

    ImTextureID LoadTextureFromFile(const std::string &filename);

    ImTextureID directoryIcon;
    ImTextureID fileIcon;

    Shader shaderProgram;
    float window_width = 0.0f;
    float window_height = 0.0f;

    std::vector<std::shared_ptr<Object>> sceneObjects;

    std::shared_ptr<Object> selectedObject = nullptr;

    bool cameraOn = false;

    void ProcessInput();

    void Render();

    void RenderUI();
    void RenderObjectList();
    void RenderPropertiesWindow();
    void RenderFileSystem();
    void RenderScene();
    void RenderGame();

    // void CreateObject(const std::string& name, const std::string& path);

    void CreatePlane();
    void AddOBJModel(const std::string &name, const std::string &path);

    void AddObject(std::shared_ptr<Object> object)
    {
        sceneObjects.push_back(std::shared_ptr<Object>(object));
    }

    void ShaderCreator();

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
