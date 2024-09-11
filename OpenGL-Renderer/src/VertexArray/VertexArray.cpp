#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &mRendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &mRendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(mRendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}
