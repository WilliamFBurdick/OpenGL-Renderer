#include "Test.h"
#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Window* window, Test*& currentTest):
		Test(window), mCurrentTest(currentTest)
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : mTests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				mCurrentTest = test.second(mWindow);
			}
		}
	}
}

