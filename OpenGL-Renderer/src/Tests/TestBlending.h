#pragma once
#include "Test.h"
#include "Camera/Camera.h"
#include "Shader/Shader.h"
#include "VertexArray/VAO.h"
#include "Model/Mesh.h"

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
		Camera mCamera;

		Texture mMarble, mFloor, mTransparent;
		std::vector<glm::vec3> mWindowPositions;

		Shader mShader;

		VAO mCubeVAO, mPlaneVAO, mTransparentVAO;
		VBO mCubeVBO, mPlaneVBO, mTransparentVBO;
	};
}

