#include "ShaderClass.hpp"

std::string LoadShaderSource(const std::string &filePath)
{
    std::
        ifstream fileStream(filePath);
    if (!fileStream.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo del shader: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

// Constructor del shader
Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    // 1. Recuperar el código fuente de los shaders desde los archivos
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    // 2. Compilar los shaders
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // Crear los shaders
    GLuint vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "Error al compilar el shader vertex\n"
                  << infoLog << std::endl;
    }


    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "Error al compilar el shader fragment\n"
                  << infoLog << std::endl;
    }

    // Programa del shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "Error al enlazar el programa del shader\n"
                  << infoLog << std::endl;
    }

    // Eliminar los shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}