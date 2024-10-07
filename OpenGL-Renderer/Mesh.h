#pragma once

#include <string>
#include <vector>
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VAO vao;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void Draw(Shader& shader, Camera& camera);
};

