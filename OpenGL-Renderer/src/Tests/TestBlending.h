#pragma once
#include "Test.h"

namespace test
{
	class TestBlending : public Test
	{
	public:
		TestBlending();
		~TestBlending();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

	};
}

