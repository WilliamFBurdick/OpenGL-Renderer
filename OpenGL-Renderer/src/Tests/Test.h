#pragma once
#include <vector>
#include <string>
#include <functional>

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() = 0 {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTest);

		virtual void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			printf("Registering test: %s\n", name.c_str());
			mTests.push_back(std::make_pair(name, []() {return new T(); }));
		}

	private:
		Test*& mCurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> mTests;
	};
}