#pragma once
#include "Test.h"

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

	};
}

