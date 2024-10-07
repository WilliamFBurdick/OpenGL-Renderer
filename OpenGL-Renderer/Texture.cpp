#include "Texture.h"

Texture::Texture(const char* path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &numChannels, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(texType, 0, GL_RGBA, width, height, 0, format, pixelType, data);
	glGenerateMipmap(texType);

	stbi_image_free(data);
	glBindTexture(texType, 0);
}

void Texture::TexUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUnit = glGetUniformLocation(shader.ID, uniform);
	shader.Use();
	glUniform1i(texUnit, unit);
}

void Texture::Bind(unsigned int slot = 0)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}