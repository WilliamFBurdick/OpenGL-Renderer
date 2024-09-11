#pragma once

#include <vector>
#include <glad/glad.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() :
		mStride(0) {}

	template <typename T>
	void Push(int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(int count)
	{
		mElements.push_back({ GL_FLOAT, (unsigned int)count, GL_FALSE });
		mStride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(int count)
	{
		mElements.push_back({ GL_UNSIGNED_INT, (unsigned int)count, GL_FALSE });
		mStride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(int count)
	{
		mElements.push_back({ GL_UNSIGNED_BYTE, (unsigned int)count, GL_TRUE });
		mStride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return mElements; }
	inline unsigned int GetStride() const { return mStride; }

private:
	std::vector<VertexBufferElement> mElements;
	unsigned int mStride;
};