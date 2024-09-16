#pragma once
#include "Test.h"
#include "Texture/Texture.h"
#include "Shader/Shader.h"
#include "VertexArray/VAO.h"
#include "Camera/Camera.h"

namespace test
{
	class TestCubemap : public Test
	{
	public:
		TestCubemap(Window* window);
		~TestCubemap();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Texture mCube, mFloor;
		unsigned int mCubemap;
		Shader mSkyboxShader, mObjectShader;
		VAO mSkyboxVAO, mCubeVAO, mPlaneVAO;
		VBO mSkyboxVBO, mCubeVBO, mPlaneVBO;

		Camera mCamera;
	};
}