#pragma once

#include<glad/glad.h>
#include<stb_image.h>

#include"ShaderClass.hpp"

class Texture
{
public:
	GLuint ID;
	GLenum type;

	// Constructor that generates a Texture ID
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	// Default constructor
	Texture() {}

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};