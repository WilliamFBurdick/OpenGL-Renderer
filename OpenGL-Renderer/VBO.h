#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position, normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO
{
public:
	VBO(std::vector<Vertex>& vertices);
	GLuint ID;

	void Bind();
	void Unbind();
	void Delete();
};

