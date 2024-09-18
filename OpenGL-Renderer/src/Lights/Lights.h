#pragma once

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambient, diffuse, specular;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient, diffuse, specular;
	float constant, linear, quadratic;
};

struct SpotLight
{
	glm::vec3 position, direction;
	glm::vec3 ambient, diffuse, specular;
	float constant, linear, quadratic;
	float cutOff, outerCutOff;
};