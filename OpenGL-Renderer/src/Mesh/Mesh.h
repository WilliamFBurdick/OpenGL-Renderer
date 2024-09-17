#pragma once
#include <string>
#include <vector>
#include "VertexArray/VAO.h"
#include "Camera/Camera.h"
#include "Texture/Texture.h"
#include "Shader/Shader.h"

class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<GLuint> Indices;
	std::vector <Texture> Textures;

	VAO VAO;
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void Draw(Shader& shader, Camera& camera);
};

