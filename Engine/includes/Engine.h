#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#define GL_SILENCE_DEPRECATION
#include "FrameBuffer.h"
#include <GLFW/glfw3.h>
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
#include "ShaderClass.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <stb_image.h>
#include <string>
#include "Texture.hpp"
#include "Camera.hpp"

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

        VAO1.Delete();
        VBO1.Delete();
        EBO1.Delete();
        brickTex.Delete();
        shaderProgram.Delete();

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
    EBO EBO1;
    VBO VBO1;
    VAO VAO1;
    VAO lightVAO;
    VBO lightVBO;
    EBO lightEBO;
    Shader shaderProgram;
    Shader lightShader;
    GLuint uniID;
    Texture brickTex;
    // Vertices coordinates

    // Vertices coordinates
    GLfloat vertices[176] =
        {
            //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
            -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // Bottom side
            -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 5.0f, 0.0f, -1.0f, 0.0f, // Bottom side
            0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 5.0f, 0.0f, -1.0f, 0.0f,  // Bottom side
            0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f,   // Bottom side

            -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, -0.8f, 0.5f, 0.0f,  // Left Side
            -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, -0.8f, 0.5f, 0.0f, // Left Side
            0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, -0.8f, 0.5f, 0.0f,   // Left Side

            -0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, 0.5f, -0.8f, // Non-facing side
            0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, 0.5f, -0.8f,  // Non-facing side
            0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f, 0.5f, -0.8f,   // Non-facing side

            0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.8f, 0.5f, 0.0f, // Right side
            0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.8f, 0.5f, 0.0f,  // Right side
            0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.8f, 0.5f, 0.0f,  // Right side

            0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 5.0f, 0.0f, 0.0f, 0.5f, 0.8f,  // Facing side
            -0.5f, 0.0f, 0.5f, 0.83f, 0.70f, 0.44f, 0.0f, 0.0f, 0.0f, 0.5f, 0.8f, // Facing side
            0.0f, 0.8f, 0.0f, 0.92f, 0.86f, 0.76f, 2.5f, 5.0f, 0.0f, 0.5f, 0.8f   // Facing side
    };

    // Indices for vertices order
    GLuint indices[18] =
        {
            0, 1, 2,    // Bottom side
            0, 2, 3,    // Bottom side
            4, 6, 5,    // Left side
            7, 9, 8,    // Non-facing side
            10, 12, 11, // Right side
            13, 15, 14  // Facing side
    };

    GLfloat lightVertices[24] =
        { //     COORDINATES     //
            -0.1f, -0.1f, 0.1f,
            -0.1f, -0.1f, -0.1f,
            0.1f, -0.1f, -0.1f,
            0.1f, -0.1f, 0.1f,
            -0.1f, 0.1f, 0.1f,
            -0.1f, 0.1f, -0.1f,
            0.1f, 0.1f, -0.1f,
            0.1f, 0.1f, 0.1f};

    GLuint lightIndices[36] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 4, 7,
            0, 7, 3,
            3, 7, 6,
            3, 6, 2,
            2, 6, 5,
            2, 5, 1,
            1, 5, 4,
            1, 4, 0,
            4, 5, 6,
            4, 6, 7};

    float window_width = 0.0f;
    float window_height = 0.0f;

    bool cameraOn = false;

    void ProcessInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (cameraOn)
            camera->Inputs(window);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
            cameraOn = false;
    }

    void Render();

    void RenderImGui();

    void ShaderCreator();

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
