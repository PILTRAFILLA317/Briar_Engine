#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>


class Shader
{
public:
    // ID del programa del shader
    GLuint ID;

    // Constructor del shader
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    // Defoult constructor
    Shader() {}
    
    // Usar el shader
    void Activate()
    {
        glUseProgram(ID);
    }

    void Delete()
    {
        glDeleteProgram(ID);
    }

    // Establecer uniformes
    void SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void SetFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
};