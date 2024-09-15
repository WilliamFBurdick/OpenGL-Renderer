#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &mRendererID);
}

void VAO::Bind()
{
	glBindVertexArray(mRendererID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::AddAttribute(VBO& VBO, GLuint location, GLuint count, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(location, count, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(location);
	VBO.Unbind();
}
