#pragma once
#include "Test.h"
#include "Texture/Texture.h"
#include "Shader/Shader.h"

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
		Texture mCubemap;
	};
}