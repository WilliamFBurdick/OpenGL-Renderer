#pragma once
#include "Test.h"
#include "VertexArray/VertexArray.h"
#include "Camera/Camera.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

namespace test
{
	class TestBlending : public Test
	{
	public:
		TestBlending(Window* window);
		~TestBlending();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		bool mSort = true;
		VertexArray mCubeVAO, mPlaneVAO, mTransparentVAO;
		Camera mCamera;

		Texture mMarble, mFloor, mTransparent;

		std::vector<glm::vec3> mWindowPositions;

		Shader mShader;
	};
}

