#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include "Shader.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void TexUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind(unsigned int slot = 0);
	void Unbind();
	void Delete();
};

