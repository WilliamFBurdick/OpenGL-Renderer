#pragma once
#include "Test.h"

#include "VertexArray/VAO.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "Camera/Camera.h"
#include "Lights/Lights.h"

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

