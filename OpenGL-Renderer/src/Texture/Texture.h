#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	void Bind(unsigned int slot = 0);

	inline int GetWidth() const { return mWidth; }
	inline int GetHeight() const { return mHeight; }
private:
	unsigned int mRendererID;
	const char* mFilePath;
	unsigned char* mLocalBuffer;
	int mWidth, mHeight, mBitsPerPixel;
};

