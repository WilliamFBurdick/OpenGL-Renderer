#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdio>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class VBO
{
public:
	VBO(std::vector<Vertex>& vertices);
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO() {}
	void Bind();
	void Unbind();
private:
	unsigned int mRendererID;
};

