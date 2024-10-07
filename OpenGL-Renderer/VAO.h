#pragma once

#include "VBO.h"

class VAO
{
public:
	VAO();
	GLuint ID;

	void LinkAttrib(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

