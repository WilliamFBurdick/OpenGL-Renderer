#pragma once
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddIndex(const IndexBuffer& ib);

	inline bool HasIndexBuffer() const { return mHasIndexBuffer; }
private:
	unsigned int mRendererID;
	bool mHasIndexBuffer;
};

