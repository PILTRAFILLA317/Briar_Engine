#pragma once

#include<string>
#include<iostream>
#include"glad/glad.h"
#include"VBO.hpp"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO() {};

    void Init()
    {
        glGenVertexArrays(1, &ID);
    }

	// Links a VBO Attribute such as a position or color to the VAO
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};
// Constructor that generates a Vertex Array Object