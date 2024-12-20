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
        ImGui::InputFloat3("Position", glm::value_ptr(selectedObject->position));
        // Rotación
        ImGui::InputFloat3("Rotation", glm::value_ptr(selectedObject->rotation));
        // Escala
        ImGui::InputFloat3("Scale", glm::value_ptr(selectedObject->scale));

        if (selectedObject->type == "SubMesh")
        {
            auto subMesh = std::dynamic_pointer_cast<SubMesh>(selectedObject);
            if (subMesh)
            {
                // Mostrar la textura del SubMesh
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
    }
    ImGui::End();
}