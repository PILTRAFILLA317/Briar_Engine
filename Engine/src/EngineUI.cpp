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

std::string TruncateText(const std::string& text, size_t maxChars) {
    if (text.length() <= maxChars) {
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
