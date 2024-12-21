#include "Engine.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "tinyfiledialogs.h"

struct FileEntry
{
    std::string name;
    bool is_directory;
};

void Engine::RenderPropertiesWindow()
{
    std::string selectedImagePath = "";

    ImGui::Begin("Object Properties"); // Ventana de propiedades
    if (selectedObject)
    {
        // Muestra el nombre del objeto
        ImGui::Text("Name: %s", selectedObject->name.c_str());

        // Posición
        ImGui::DragFloat3("Position", glm::value_ptr(selectedObject->position));
        // Rotación
        ImGui::DragFloat3("Rotation", glm::value_ptr(selectedObject->rotation));
        // Escala
        ImGui::DragFloat3("Scale", glm::value_ptr(selectedObject->scale));

        ImGui::Separator();
        if (selectedObject->type == "SubMesh")
        {
            auto subMesh = std::dynamic_pointer_cast<SubMesh>(selectedObject);
            if (subMesh)
            {
                // Mostrar la textura del SubMesh
                ImGui::Text("Texture");
                if (subMesh->texture.ID == -1)
                    ImGui::Image((ImTextureID)LoadTextureFromFile(basePath + "/assets/textures/folder.png"), ImVec2(100, 100));
                else
                    ImGui::Image((ImTextureID)subMesh->texture.ID, ImVec2(100, 100));
                if (ImGui::Button("Change Texture"))
                {
                    const char *file = tinyfd_openFileDialog(
                        "Choose an image", "",
                        0, nullptr, nullptr, 0);

                    if (file != nullptr)
                    {
                        // Guardar la ruta seleccionada
                        selectedImagePath = file;
                        subMesh->UpdateTexture(selectedImagePath);
                    }
                }
            }
        }
        if (selectedObject->type == "Light")
        {
            auto light = std::dynamic_pointer_cast<Light>(selectedObject);
            if (light)
            {
                // Tipo de luz
                ImGui::Text("Light Type");
                ImGui::RadioButton("Directional", (int *)&light->lightType, Light::DIRECTIONAL);
                ImGui::SameLine();
                ImGui::RadioButton("Point", (int *)&light->lightType, Light::POINT);
                ImGui::SameLine();
                ImGui::RadioButton("Spot", (int *)&light->lightType, Light::SPOT);

                // Color
                ImGui::ColorEdit3("Color", glm::value_ptr(light->color));
                // Intensidad
                ImGui::DragFloat("Intensity", &light->intensity, 0.1f, 0.0f, 100.0f);
                // Rango
                if (light->lightType == Light::POINT || light->lightType == Light::SPOT)
                {
                    ImGui::DragFloat("Range", &light->range, 0.1f, 0.0f, 100.0f);
                }
            }
        }
    }
    ImGui::End();
}