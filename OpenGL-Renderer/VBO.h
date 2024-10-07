#pragma once

#include <glad/glad.h>

class VBO
{
public:
	VBO(GLfloat* vertices, GLsizeiptr size);
	GLuint ID;

	void Bind();
	void Unbind();
	void Delete();
};

