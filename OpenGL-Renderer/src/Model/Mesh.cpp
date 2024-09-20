#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures):
	Vertices(vertices), Indices(indices), Textures(textures)
{
	VAO.Bind();
	VBO VBO(Vertices);
	EBO EBO(Indices);

	VAO.AddAttribute(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.AddAttribute(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.AddAttribute(VBO, 0, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader)
{
	shader.use();
	VAO.Bind();

	unsigned int nrDiffuse = 0;
	unsigned int nrSpecular = 0;

	for (unsigned int i = 0; i < Textures.size(); i++)
	{
		std::string uniform;
		switch (Textures[i].GetType())
		{
		case TextureType::Diffuse:
			uniform = "diffuse_" + std::to_string(nrDiffuse++);
			break;
		case TextureType::Specular:
			uniform = "specular_" + std::to_string(nrSpecular++);
			break;
		default:
			break;
		}
		shader.SetInt("material." + uniform, i);
		Textures[i].Bind(i);
	}
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}
