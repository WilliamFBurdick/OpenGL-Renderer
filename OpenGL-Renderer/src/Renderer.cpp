#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.use();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const VertexArray& va, const int count, const Shader& shader) const
{
	shader.use();
	va.Bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::SetClearColor(glm::vec4 clearColor) const
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
}
