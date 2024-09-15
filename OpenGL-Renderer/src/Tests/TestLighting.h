#pragma once
#include "Test.h"

#include "VertexArray/VAO.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "Camera/Camera.h"

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

namespace test
{
    class TestLighting : public Test
    {
	public:
		TestLighting(Window* window);
		~TestLighting();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Shader mPhongShader, mColorShader;
		Texture mCube_D, mCube_S, mFloor_D;
		VAO mCubeVAO, mPlaneVAO;
		VBO mCubeVBO, mPlaneVBO;

		DirectionalLight mDirLight;
		std::vector<PointLight> mPointLights;
		std::vector<SpotLight> mSpotLights;

		Camera mCamera;

		void ApplyLighting(Shader& shader);
    };
}

