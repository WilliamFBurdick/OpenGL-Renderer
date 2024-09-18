#pragma once
#include <glad/glad.h>
#include <vector>

class Texture
{
public:
	enum class Type
	{
		Diffuse, Specular, Buffer
	};

	// Constructor for loading single texture
	Texture(const char* path, Type type);
	// Constructor for generating texture (created empty)
	Texture(GLenum format, unsigned int width, unsigned int height);
	Texture() {}
	~Texture();

	void Bind(unsigned int slot = 0);

	inline unsigned int GetID() const { return mRendererID; }
	inline Type GetType() const { return mType; }
private:
	unsigned int mRendererID;
	Type mType;
};

