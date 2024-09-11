#include "Texture.h"
#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture(const char* path):
	mFilePath(path)
{
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &mRendererID);
	mLocalBuffer = stbi_load(path, &mWidth, &mHeight, &mBitsPerPixel, 0);
	if (mLocalBuffer)
	{
		GLenum format;
		if (mBitsPerPixel == 1)
			format = GL_RED;
		if (mBitsPerPixel == 3)
			format = GL_RGB;
		if (mBitsPerPixel == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, mRendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuffer);
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
		stbi_image_free(mLocalBuffer);
	}
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
