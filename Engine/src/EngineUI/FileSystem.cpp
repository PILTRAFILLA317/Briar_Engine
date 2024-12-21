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

std::string TruncateText(const std::string &text, size_t maxChars)
{
    if (text.length() <= maxChars)
    {
        return text;
    }
    return text.substr(0, maxChars - 3) + "..."; // Truncar y añadir '...'
}

std::vector<FileEntry> ListDirectory(const std::string &path)
{
    std::vector<FileEntry> entries;
    for (const auto &entry : fs::directory_iterator(path))
    {
        entries.push_back({entry.path().filename().string(),
                           entry.is_directory()});
    }
    return entries;
}

void Engine::RenderFileSystem()
{
    static std::string currentPath = basePath;
    ImGui::Begin("File System");

    // Botón para volver al directorio padre.
    if (ImGui::Button("Up") && currentPath != basePath)
    {
        currentPath = fs::path(currentPath).parent_path().string();
    }

    ImGui::Text("Current Path: %s", currentPath.c_str());
    ImGui::Separator();

    // Listar archivos y carpetas.
    auto entries = ListDirectory(currentPath);

    // Configurar tamaño de icono
    constexpr float iconSize = 50.0f; // Tamaño de los iconos (ancho y alto)
    constexpr float padding = 10.0f;  // Espaciado entre elementos
    constexpr size_t maxTextLength = 8;

    // Calcular número máximo de elementos por fila según el ancho de la ventana
    float fswindowWidth = ImGui::GetContentRegionAvail().x; // Ancho disponible
    size_t itemsPerRow = static_cast<size_t>(fswindowWidth / (iconSize + padding));

    if (itemsPerRow < 1)
        itemsPerRow = 1; // Asegurar al menos 1 por fila

    size_t itemIndex = 0; // Índice del elemento actual
    for (const auto &entry : entries)
    {
        ImGui::BeginGroup(); // Agrupar botón e texto juntos

        // Dibujar el botón (icono)
        if (entry.is_directory)
            ImGui::ImageButton((ImTextureID)directoryIcon, ImVec2(iconSize, iconSize));
        else
            ImGui::ImageButton((ImTextureID)fileIcon, ImVec2(iconSize, iconSize));

        // Detectar clic en el botón
        if (ImGui::IsItemClicked() && entry.is_directory)
        {
            currentPath = fs::path(currentPath) / entry.name;
        }
        else if (ImGui::IsItemClicked() && !entry.is_directory)
        {
            if (ImGui::IsItemClicked() && entry.name.ends_with(".obj"))
                AddOBJModel(entry.name, fs::path(currentPath).string());
        }

        ImGui::Dummy(ImVec2(0.0f, 0.5f)); // Espacio entre el botón y el texto
        std::string truncatedName = TruncateText(entry.name, maxTextLength);
        ImGui::TextWrapped("%s", truncatedName.c_str());

        ImGui::EndGroup();

        // Alinear en la misma fila si no es el último de la fila
        if (++itemIndex % itemsPerRow != 0)
        {
            ImGui::SameLine(0.0f, padding);
        }
    }

    ImGui::End();
}
