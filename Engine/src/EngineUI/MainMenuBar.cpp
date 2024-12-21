#include "Engine.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


void Engine::RenderMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Create"))
            {
            }
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
            }
            if (ImGui::MenuItem("Save as.."))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {
            }
            if (ImGui::MenuItem("Redo", "CTRL+Y"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Scene"))
            {
            }
            if (ImGui::MenuItem("Game"))
            {
            }
            if (ImGui::MenuItem("Properties"))
            {
            }
            if (ImGui::MenuItem("File System"))
            {
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Objects"))
        {
            if (ImGui::MenuItem("Create Empty"))
            {
                Object *newObject = new Object("New Object");
                AddObject(std::shared_ptr<Object> (newObject));
            }
            if (ImGui::MenuItem("Create Light"))
            {
                Light *newLight = new Light("New Light", Light::POINT, glm::vec3(1.0f), 1.0f);
                AddObject(std::shared_ptr<Object> (newLight));
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}