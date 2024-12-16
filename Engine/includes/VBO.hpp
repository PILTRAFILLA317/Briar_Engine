#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};



class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(std::vector<Vertex>& vertices);

	// VBO() {}

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

// Constructor that generates a Vertex Buffer Object and links it to vertices