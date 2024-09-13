#pragma once
#include <vector>
#include <string>
#include <functional>
#include "Window/Window.h"

namespace test
{
	class Test
	{
	public:
		Test(Window* window): mWindow(window) {}
		virtual ~Test() = 0 {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	protected:
		Window* mWindow;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Window* window, Test*& currentTest);

		virtual void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			printf("Registering test: %s\n", name.c_str());
			mTests.push_back(std::make_pair(name, [](Window* window) {return new T(window); }));
		}

	private:
		Test*& mCurrentTest;
		std::vector<std::pair<std::string, std::function<Test*(Window*)>>> mTests;
	};
}