#pragma once

#include <glad/glad.h>
#include <stb_image.h>

#include "ShaderClass.hpp"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	Texture() {
		ID = -1;
		type = GL_TEXTURE_2D;
	}

	// = operator overloading
	Texture &operator=(const Texture &tex)
	{
		this->ID = tex.ID;
		this->type = tex.type;
		return *this;
	}

	// Assigns a texture unit to a texture
	void texUnit(Shader &shader, const char *uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};