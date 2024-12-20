#include "Engine.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

struct FileEntry
{
    std::string name;
    bool is_directory;
};

void Engine::RenderObjectList()
{
    ImGui::Begin("Object List"); // Ventana de la lista de objetos

    for (size_t i = 0; i < sceneObjects.size(); ++i)
    {
        bool isMesh = false;
        std::shared_ptr<Mesh> meshPtr = std::dynamic_pointer_cast<Mesh>(sceneObjects[i]);

        if (meshPtr)
        {
            isMesh = true;
        }

        // Crear un identificador único para el Mesh (usando el nombre o índice)
        std::string meshID = "##Mesh" + std::to_string(i);

        if (isMesh)
        {
            // Crear el TreeNode para el Mesh, pero usando ImGui::Selectable dentro de él
            bool node_selected = selectedObject == sceneObjects[i]; // Verificar si el nodo está seleccionado
            if (ImGui::TreeNodeEx(meshID.c_str(), ImGuiTreeNodeFlags_Selected, sceneObjects[i]->name.c_str()))
            {
                // Al hacer clic en el TreeNode, seleccionamos el Mesh principal
                if (ImGui::IsItemClicked())
                {
                    selectedObject = sceneObjects[i];
                }

                // Mostrar los SubMeshes dentro de este TreeNode
                for (size_t j = 0; j < meshPtr->subMeshes.size(); ++j)
                {
                    // Crear un identificador único para el SubMesh
                    std::string subMeshID = "##SubMesh" + std::to_string(i) + "_" + std::to_string(j);

                    // Listar los SubMeshes como elementos seleccionables dentro del Mesh
                    if (ImGui::Selectable(meshPtr->subMeshes[j].name.c_str(), false))
                    {
                        selectedObject = std::shared_ptr<Object>(&meshPtr->subMeshes[j], [](Object *) {});
                        // selectedObject = std::shared_ptr<SubMesh>(&meshPtr->subMeshes[j]), [](SubMesh *) {};
                    }
                }

                // Cerrar el TreeNode
                ImGui::TreePop();
            }
            else if (node_selected)
            {
                // Asegurarnos de que el nodo seleccionado se resalte
                ImGui::SetItemDefaultFocus();
            }
        }
        else
        {
            // Si no es un Mesh, solo mostrar el objeto
            if (ImGui::Selectable(sceneObjects[i]->name.c_str(), selectedObject == sceneObjects[i]))
            {
                selectedObject = sceneObjects[i];
            }
        }
    }
    ImGui::End();
}