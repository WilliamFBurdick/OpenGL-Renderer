#include "Texture.h"
#include <stb_image.h>

Texture::Texture(const char* path, Type type):
	mType(type)
{
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &mRendererID);
	int width, height, bitsPerPixel;
	unsigned char* data = stbi_load(path, &width, &height, &bitsPerPixel, 0);
	if (data)
	{
		GLenum format;
		if (bitsPerPixel == 1)
			format = GL_RED;
		if (bitsPerPixel == 3)
			format = GL_RGB;
		if (bitsPerPixel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, mRendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		printf("Failed to load texture at path: %s\n", path);
		stbi_image_free(data);
	}
}

Texture::Texture(GLenum format, unsigned int width, unsigned int height):
{
	mType = Texture::Type::Buffer;
	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRendererID, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mRendererID);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
}
