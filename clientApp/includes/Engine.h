#pragma once
#include "Camera.hpp"
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
    Shader shaderProgram;

    void ProcessInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Procesar movimientos de la cámara
        if (camera)
        {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera->ProcessKeyboardInput(GLFW_KEY_W, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera->ProcessKeyboardInput(GLFW_KEY_S, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera->ProcessKeyboardInput(GLFW_KEY_A, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera->ProcessKeyboardInput(GLFW_KEY_D, deltaTime);
        }
    }

    void Render();

    void RenderImGui();

    void ShaderCreator()
    {
        // Vertices coordinates
        GLfloat vertices[] =
            {
                -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower left corner
                0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,     // Lower right corner
                0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  // Upper corner
                -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
                0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,  // Inner right
                0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f      // Inner down
            };

        // Indices for vertices order
        GLuint indices[] =
            {
                0, 3, 5, // Lower left triangle
                3, 2, 4, // Upper triangle
                5, 4, 1  // Lower right triangle
            };
        shaderProgram = Shader("clientApp/shaders/vertex_shader.glsl", "clientApp/shaders/fragment_shader.glsl");
        VAO1.Init();
        VAO1.Bind();
        // Generates Vertex Buffer Object and links it to vertices
        VBO1 = VBO(vertices, sizeof(vertices));
        // Generates Element Buffer Object and links it to indices
        EBO1 = EBO(indices, sizeof(indices));

        // Links VBO to VAO
        VAO1.LinkVBO(VBO1, 0);
        // Unbind all to prevent accidentally modifying them
        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();
    }

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
