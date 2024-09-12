#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "Window/Window.h"
#include "Tests/Test.h"
#include "Tests/TestBlending.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char* argv[])
{
	Window window;
	Renderer renderer;

	if (!window.Init())
		return -1;

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestBlending>("Blending");

	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;;
		lastFrame = currentFrame;

		renderer.Clear();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (currentTest)
		{
			currentTest->OnUpdate(deltaTime);
			currentTest->OnRender();
			ImGui::Begin("test");
			currentTest->OnImGuiRender();
			if (currentTest != testMenu && ImGui::Button("<-"))
			{
				delete currentTest;
				currentTest = testMenu;
			}
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	delete currentTest;
	if (testMenu)
		delete testMenu;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}