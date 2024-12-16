#include "Engine.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Engine::RenderObjectList()
{
    ImGui::Begin("Object List"); // Ventana de la lista de objetos
    for (size_t i = 0; i < sceneObjects.size(); ++i)
    {
        // Resalta el objeto seleccionado en la lista
        if (ImGui::Selectable(sceneObjects[i]->name.c_str(), selectedObject == sceneObjects[i]))
        {
            selectedObject = sceneObjects[i];
        }
    }
    ImGui::End();
}

void Engine::RenderPropertiesWindow()
{
    ImGui::Begin("Object Properties"); // Ventana de propiedades
    if (selectedObject)
    {
        // Muestra el nombre del objeto
        ImGui::Text("Name: %s", selectedObject->name.c_str());

        // Posición
        if (ImGui::InputFloat3("Position", glm::value_ptr(selectedObject->position)))
        {
            // Actualizar la posición en caso de cambios
        }

        // Rotación
        if (ImGui::InputFloat3("Rotation", glm::value_ptr(selectedObject->rotation)))
        {
            // Actualizar la rotación en caso de cambios
        }

        // Escala
        if (ImGui::InputFloat3("Scale", glm::value_ptr(selectedObject->scale)))
        {
            // Actualizar la escala en caso de cambios
        }
    }
    ImGui::End();
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

void Engine::RenderFileSystem()
{
    ImGui::Begin("File System"); // Ventana del sistema de archivos

    // Simulación de recursos (puedes implementar un explorador real más adelante)
    if (ImGui::TreeNode("Available Resources"))
    {
        if (ImGui::Button("Add Cube"))
        {
            // Añadir un cubo a la escena
            // AddObjectToScene(new Object("Cube"));
        }
        if (ImGui::Button("Add Sphere"))
        {
            // Añadir una esfera a la escena
            // AddObjectToScene(new Object("Sphere"));
        }
        ImGui::TreePop();
    }

    ImGui::End();
}
