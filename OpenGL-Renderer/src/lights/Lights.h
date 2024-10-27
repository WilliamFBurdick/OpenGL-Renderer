#pragma once
#include "shader/Shader.h"

struct Attenuation
{
	float constant, linear, quadratic;
};

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambient, diffuse, specular;
	unsigned int depthMapFBO;
	unsigned int depthMapTexture;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient, diffuse, specular;
	Attenuation attenuation;
	unsigned int depthMapFBO;
	unsigned int depthCubemap;
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient, diffuse, specular;
	float cutOff, outerCutOff;
	Attenuation attenuation;
	unsigned int depthMapFBO;
	unsigned int depthCubemap;
};

class Light
{
public:
	static void ApplyDirectionalLight(Shader*& shader, DirectionalLight light, int index = 0)
	{
		std::string id = "directionalLights[" + std::to_string(index) + "]";
		shader->setVec3(id + ".direction", light.direction);
		shader->setVec3(id + ".ambient", light.ambient);
		shader->setVec3(id + ".diffuse", light.diffuse);
		shader->setVec3(id + ".specular", light.specular);
	}

	static void ApplyPointLight(Shader*& shader, PointLight light, int index = 0)
	{
		std::string id = "pointLights[" + std::to_string(index) + "]";
		shader->setVec3(id + ".position", light.position);
		shader->setVec3(id + ".ambient", light.ambient);
		shader->setVec3(id + ".diffuse", light.diffuse);
		shader->setVec3(id + ".specular", light.specular);
		shader->setFloat(id + ".attenuation.constant", light.attenuation.constant);
		shader->setFloat(id + ".attenuation.linear", light.attenuation.linear);
		shader->setFloat(id + ".attenuation.quadratic", light.attenuation.quadratic);
	}

	static void ApplySpotLight(Shader*& shader, SpotLight light, int index = 0)
	{
		std::string id = "spotLights[" + std::to_string(index) + "]";
		shader->setVec3(id + ".position", light.position);
		shader->setVec3(id + ".direction", light.direction);
		shader->setVec3(id + ".ambient", light.ambient);
		shader->setVec3(id + ".diffuse", light.diffuse);
		shader->setVec3(id + ".specular", light.specular);
		shader->setFloat(id + ".attenuation.constant", light.attenuation.constant);
		shader->setFloat(id + ".attenuation.linear", light.attenuation.linear);
		shader->setFloat(id + ".attenuation.quadratic", light.attenuation.quadratic);
		shader->setFloat(id + ".cutOff", light.cutOff);
		shader->setFloat(id + ".outerCutOff", light.outerCutOff);
	}
};