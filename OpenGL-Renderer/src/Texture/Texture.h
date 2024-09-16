#pragma once
#include <glad/glad.h>
#include <vector>

class Texture
{
public:
	// Constructor for loading single texture
	Texture(const char* path);
	// Constructor for generating texture (created empty)
	Texture(GLenum format, unsigned int width, unsigned int height);
	Texture() {}
	~Texture();

	void Bind(unsigned int slot = 0);

	inline int GetWidth() const { return mWidth; }
	inline int GetHeight() const { return mHeight; }

	inline unsigned int GetID() const { return mRendererID; }
private:
	unsigned int mRendererID;
	const char* mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBitsPerPixel;
};

