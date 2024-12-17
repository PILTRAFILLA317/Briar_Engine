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
        lightShader.Delete();

        glfwDestroyWindow(window);
        glfwTerminate();
        delete camera;
        delete framebuffer;
    }

private:
    int windowWidth, windowHeight;
    std::string windowTitle;
    GLFWwindow *window = nullptr;
    Camera *camera = nullptr;
    FrameBuffer *framebuffer = nullptr;
    color clearColor = {0.1f, 0.1f, 0.1f, 1.0f};
    float deltaTime = 0.0f, lastFrame = 0.0f;
    Shader shaderProgram;
    Shader lightShader;
    GLuint uniID;
    float window_width = 0.0f;
    float window_height = 0.0f;
    Mesh mesh;

    std::vector<Object*> sceneObjects;
    Object* selectedObject = nullptr;  // Objeto actualmente seleccionado

    bool cameraOn = false;

    void ProcessInput();

    void Render();

    void RenderUI();
    void RenderObjectList();
    void RenderPropertiesWindow();
    void RenderFileSystem();
    void RenderScene();
    void RenderGame();

    void ShaderCreator();

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
