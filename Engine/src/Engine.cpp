#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Engine::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (cameraOn)
        camera->Inputs(window);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        cameraOn = false;
}

void Engine::ShaderCreator()
{
    std::string parentDir = (fs::current_path()).string();
    std::string shaderPath = "/assets/shaders/";

    shaderProgram = Shader((parentDir + shaderPath + "defaultVert.glsl"), (parentDir + shaderPath + "defaultFrag.glsl"));
    textureShader = Shader((parentDir + shaderPath + "textureVert.glsl"), (parentDir + shaderPath + "textureFrag.glsl"));

    camera = new Camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glDisable(GL_CULL_FACE);
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

    // Configurar framebuffer

    basePath = fs::current_path().string();
    directoryIcon = LoadTextureFromFile(basePath + "/assets/textures/folder.png");
    fileIcon = LoadTextureFromFile(basePath + "/assets/textures/file.png");
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
        RenderUI();
        // Renderizar escena
        Render();
        glfwSwapBuffers(window);
    }
}

void Engine::RenderUI()
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
    ImGui::Text("Camera Orientation: (%.1f, %.1f, %.1f)",
                camera->Orientation.x, camera->Orientation.y, camera->Orientation.z);
    ImGui::Text("Camera On: %s", cameraOn ? "true" : "false");
    ImGui::ColorEdit3("Color", (float *)&clearColor);
    ImGui::End();

    RenderMainMenuBar();
    RenderObjectList();
    RenderPropertiesWindow();
    RenderFileSystem();
    RenderGame();
    RenderScene();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Render()
{
    framebuffer->Bind();

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera
    camera->updateMatrix(45.0f, 0.1f, 1000.0f);
    for (auto &object : sceneObjects)
    {
        if (object->type == "Mesh")
            object->Draw(shaderProgram, *camera);
        if (object->type == "Light")
        {
            shaderProgram.Activate();
            auto light = std::dynamic_pointer_cast<Light>(object);
            // if (light)
            // {
            //     std::string base = "lights[" + std::to_string(i) + "]"; // Para acceder a cada luz

            //     // Configurar la posición, el color y la intensidad de cada luz
            //     glUniform3fv(glGetUniformLocation(shaderProgram.ID, (base + ".position").c_str()), 1, glm::value_ptr(light->position));
            //     glUniform3fv(glGetUniformLocation(shaderProgram.ID, (base + ".color").c_str()), 1, glm::value_ptr(light->color));
            //     glUniform1f(glGetUniformLocation(shaderProgram.ID, (base + ".intensity").c_str()), light->intensity);
            //     i++;
            // }
            glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightPos"), 1, glm::value_ptr(light->position));;
            glUniform3fv(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, glm::value_ptr(light->color));
            // glUniform1f(glGetUniformLocation(shaderProgram.ID, "lightIntensity"), object->intensity);
            object->Draw(textureShader, *camera);
        }
    }

    // Take care of all GLFW events
    glfwPollEvents();

    framebuffer->Unbind();
}