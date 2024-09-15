#pragma once
#include <glad/glad.h>
#include "Buffers/VBO.h"

class VAO
{
public:
	VAO();
	void Bind();
	void Unbind();

	void AddAttribute(VBO& VBO, GLuint location, GLuint count, GLenum type, GLsizeiptr stride, void* offset);
private:
	unsigned int mRendererID;
};