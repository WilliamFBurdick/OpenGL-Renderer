#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include "Shader.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	Texture(const char* path, const char* texType, GLenum slot);

	void TexUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind(unsigned int slot = 0);
	void Unbind();
	void Delete();
};

