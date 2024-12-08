#pragma once
#include "Camera.hpp"
#include "FrameBuffer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "/opt/homebrew/opt/glm/include/glm/glm.hpp"
#include "/opt/homebrew/opt/glm/include/glm/gtc/matrix_transform.hpp"
#include "/opt/homebrew/opt/glm/include/glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// Añade otras dependencias según sea necesario

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

        // Inicializar ImGui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

        // Configurar cámara
        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

            // Procesar entrada
            ProcessInput();

            // Renderizar ImGui
            RenderImGui();
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

    float deltaTime = 0.0f, lastFrame = 0.0f;

    void ProcessInput()
    {
        glfwPollEvents();
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

    void Render()
    {
        framebuffer->Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Configurar matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)windowWidth / (float)windowHeight,
                                                0.1f, 100.0f);
        glm::mat4 view = camera->GetViewMatrix();

        // Enviar matrices a los shaders y renderizar

        framebuffer->Unbind();
    }

    void RenderImGui()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        // Ventanas ImGui
        ImGui::Begin("Engine Stats");
        ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
        ImGui::End();

        ImGui::Begin("Scene");
        {
            const float window_width = ImGui::GetContentRegionAvail().x;
            const float window_height = ImGui::GetContentRegionAvail().y;
            ImGui::BeginChild("GameRender");
            std::cout << "Window width: " << window_width << std::endl;
            std::cout << "Window height: " << window_height << std::endl;
            framebuffer->RescaleFrameBuffer(window_width, window_height);
            glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);
            ImGui::Image(
                (ImTextureID)framebuffer->getFrameTexture(),
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1),
                ImVec2(1, 0));
        }
        ImGui::EndChild();
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};
