#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Engine::ShaderCreator()
{
    shaderProgram = Shader("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl");
    VAO1.Init();
    VAO1.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO1 = VBO(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO1 = EBO(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    std::string parentDir = (fs::current_path()).string();
    printf("Parent directory: %s\n", parentDir.c_str());
    std::string texPath = "/assets/textures/";
    printf("Texture path: %s\n", texPath.c_str());

    // Texture
    brickTex = Texture((parentDir + texPath + "brick2.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    printf("Texture path: %s\n", (parentDir + texPath + "brick2.png").c_str());
    brickTex.texUnit(shaderProgram, "tex0", 0);
    glEnable(GL_DEPTH_TEST);
}

void Engine::Init()
{
    // Inicialización de GLFW
    if (!glfwInit())
    {
        throw std::runtime_error("Error al inicializar GLFW");
    }

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
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
    // camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    camera = new Camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    // Configurar framebuffer
    framebuffer = new FrameBuffer(windowWidth, windowHeight);
    ShaderCreator();
    std::cout << "Engine inicializado correctamente.\n";
}

void Engine::Run()
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
        // Renderizar escena
        Render();
        glfwSwapBuffers(window);
    }
}

void Engine::RenderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport();

    // Ventanas ImGui
    ImGui::Begin("Engine Stats");
    ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
    ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)",
                camera->Position.x, camera->Position.y, camera->Position.z);
    ImGui::ColorEdit3("Color", (float *)&clearColor);
    ImGui::End();

    ImGui::Begin("Scene");
    {
        window_width = ImGui::GetContentRegionAvail().x;
        window_height = ImGui::GetContentRegionAvail().y;
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

void Engine::Render()
{
    framebuffer->Bind();

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram.Activate();

    // Camera
    camera->Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

    // Binds texture so that is appears in rendering
    brickTex.Bind();
    // Bind the VAO so OpenGL knows to use it
    VAO1.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();

    framebuffer->Unbind();
}