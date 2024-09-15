#pragma once
#include "Test.h"
#include "VertexArray/VAO.h"
#include "Texture/Texture.h"
#include "Shader/Shader.h"
#include "Camera/Camera.h"

namespace test
{
	class TestPostProcessing : public Test
	{
	public:
		TestPostProcessing(Window* window);
		~TestPostProcessing();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		VAO mCubeVAO, mPlaneVAO, mQuadVAO;
		VBO mCubeVBO, mPlaneVBO, mQuadVBO;
		Texture mMarble, mFloor;
		Texture mColorbuffer;
		Shader mObjectShader, mPostProcessingShader;

		Camera mCamera;

		int mEnabledEffect;
	};
}

