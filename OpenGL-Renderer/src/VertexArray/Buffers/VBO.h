#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdio>

class VBO
{
public:
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO() {}
	void Bind();
	void Unbind();
private:
	unsigned int mRendererID;
};

