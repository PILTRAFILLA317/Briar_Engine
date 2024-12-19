#include "Engine.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

ImTextureID Engine::LoadTextureFromFile(const std::string &filename)
{
    // Cargar la imagen desde el archivo
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return nullptr;
    }

    // Crear una textura OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Configurar los parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Subir los datos de la imagen a la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Liberar la imagen cargada
    stbi_image_free(data);

    return (ImTextureID)(intptr_t)textureID;
}

void Engine::RenderScene()
{
    ImGui::Begin("Scene");
    {
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            cameraOn = true; // Activa la cámara
        window_width = ImGui::GetContentRegionAvail().x;
        window_height = ImGui::GetContentRegionAvail().y;
        ImGui::BeginChild("GameRender");
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
}

void Engine::RenderGame()
{
    ImGui::Begin("Game"); // Ventana del juego

    // Configurar el tamaño del viewport para el render del juego
    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Text("Game Viewport");

    // Similar al render de escena, utiliza tu framebuffer de juego
    GLuint gameTexture = 0; // Cambiar a tu textura de framebuffer
    ImGui::Image((void *)(intptr_t)gameTexture, size, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}