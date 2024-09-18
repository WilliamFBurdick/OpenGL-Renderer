#pragma once
#include <glad/glad.h>
#include <vector>

enum class TextureType
{
	Diffuse, Specular, Buffer
};

class Texture
{
public:

	// Constructor for loading single texture
	Texture(const char* path, TextureType type = TextureType::Diffuse);
	// Constructor for generating texture (created empty)
	Texture(GLenum format, unsigned int width, unsigned int height);
	Texture() {}
	~Texture();

	void Bind(unsigned int slot = 0);

	inline unsigned int GetID() const { return mRendererID; }
	inline TextureType GetType() const { return mType; }
private:
	unsigned int mRendererID;
	TextureType mType;
};

