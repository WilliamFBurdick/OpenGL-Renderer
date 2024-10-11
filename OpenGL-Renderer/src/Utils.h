#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <cstdio>

class Utils
{
public:
	static unsigned int LoadTexture(const char* path)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		int width, height, numComponents;
		unsigned char* data = stbi_load(path, &width, &height, &numComponents, 0);
		if (data)
		{
			GLenum format;
			if (numComponents == 1)
				format = GL_RED;
			else if (numComponents == 3)
				format = GL_RGB;
			else if (numComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			printf("Failed to load texture at path: %s\n", path);
			stbi_image_free(data);
		}
		return textureID;
	}
};