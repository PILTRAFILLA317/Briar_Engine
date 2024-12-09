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

struct color
{
    GLfloat r, g, b, a;
};

class Engine
{
public:
    Engine(int width, int height, const std::string &title)
        : windowWidth(width), windowHeight(height), windowTitle(title) {}

    void Init()
    {
        // Inicialización de GLFW
        if (!glfwInit())
        {
            throw std::runtime_error("Error al inicializar GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Crear ventana
        window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("Error al crear la ventana GLFW");
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

        glViewport(0, 0, windowWidth, windowHeight);

        // Inicializar GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Error al inicializar GLAD");
        }

        // Inicializar ImGui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

        // Configurar cámara
        camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

        // Configurar framebuffer
        framebuffer = new FrameBuffer(windowWidth, windowHeight);

        std::cout << "Engine inicializado correctamente.\n";
    }

    void Run()
    {
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwPollEvents();

            // Procesar entrada
            ProcessInput();

            // Renderizar ImGui
            RenderImGui();

            ShaderCreator();

            // Renderizar escena
            Render();

            glfwSwapBuffers(window);
        }
    }

    ~Engine()
    {
        // Limpieza
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

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
    GLuint shaderProgram;
    GLuint VAO, VBO;

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
        // Definir los shaders
        const char *vertexShaderSource = "#version 330 core\n"
                                         "layout (location = 0) in vec3 aPos;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                         "}\0";
        const char *fragmentShaderSource = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                           "}\0";

        // Crear los shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Crear el programa de shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Eliminar los shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Vertices coordinates
        GLfloat vertices[] =
            {
                -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,  // Lower left corner
                0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,   // Lower right corner
                0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
            };

        // Create reference containers for the Vartex Array Object and the Vertex Buffer Object
        // Generate the VAO and VBO with only 1 object each
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Make the VAO the current Vertex Array Object by binding it
        glBindVertexArray(VAO);

        // Bind the VBO specifying it's a GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Introduce the vertices into the VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        // Enable the Vertex Attribute so that OpenGL knows to use it
        glEnableVertexAttribArray(0);

        // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
