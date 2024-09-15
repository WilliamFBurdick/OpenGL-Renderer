#include "Texture.h"
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
		glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, mLocalBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		mTextureType = GL_TEXTURE_2D;
	}
	else
	{
		printf("Failed to load texture at path: %s\n", path);
		stbi_image_free(mLocalBuffer);
	}
}

Texture::Texture(GLenum format, unsigned int width, unsigned int height):
	mWidth(width), mHeight(height), mLocalBuffer(), mBitsPerPixel(3), mFilePath()
{
	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_2D, mRendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRendererID, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	mTextureType = GL_TEXTURE_2D;
}

Texture::Texture(std::vector<const char*> paths)
{
	glGenTextures(1, &mRendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		mLocalBuffer = stbi_load(paths[i], &mWidth, &mHeight, &mBitsPerPixel, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mLocalBuffer);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	mTextureType = GL_TEXTURE_CUBE_MAP;
}

Texture::~Texture()
{
	glDeleteTextures(1, &mRendererID);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(mTextureType, mRendererID);
}
