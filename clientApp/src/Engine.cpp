#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string LoadShaderSource(const std::string &filePath)
{
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo del shader: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
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

void Engine::Render()
{
    framebuffer->Bind();

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // Draw the triangle using the GL_TRIANGLES primitive
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();

    // Configurar matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)windowWidth / (float)windowHeight,
                                            0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();

    // Enviar matrices a los shaders y renderizar

    framebuffer->Unbind();
}