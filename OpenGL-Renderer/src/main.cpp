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

#include "Camera/Camera.h"
#include "Input/Input.h"

#include "Window/Window.h"
#include "Tests/Test.h"
#include "Tests/TestBlending.h"
#include "Tests/TestPostProcessing.h"
#include "Tests/TestLighting.h"

const int PROPERTIES_WIDTH = 250;
const int DEMO_SELECTION_WIDTH = 250;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void RenderPropertiesTab(Window& window, test::Test*& test);
void RenderDemoList(Window& window, test::TestMenu*& testMenu);

int main(int argc, char* argv[])
{
	Window* window = new Window(1280, 720, "OpenGL Renderer");

	if (!window->Init())
		return -1;

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(window, currentTest);
	currentTest = testMenu;

	testMenu->RegisterTest<test::TestLighting>("Lighting");
	testMenu->RegisterTest<test::TestBlending>("Blending");
	testMenu->RegisterTest<test::TestPostProcessing>("Post Processing");

	while (!glfwWindowShouldClose(window->GetWindow()))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (currentTest)
		{
			currentTest->OnUpdate(deltaTime);
			//window->SetViewport(DEMO_SELECTION_WIDTH, 0, window->GetWidth() - DEMO_SELECTION_WIDTH - PROPERTIES_WIDTH, window->GetHeight());
			currentTest->OnRender();
			RenderPropertiesTab(*window, currentTest);
			RenderDemoList(*window, testMenu);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window->GetWindow());
		glfwPollEvents();
		Input::Update(window->GetWindow());
	}

	delete currentTest;
	if (testMenu != currentTest)
		delete testMenu;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void RenderPropertiesTab(Window& window, test::Test*& test)
{
	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImVec2 panelPos = ImVec2(window.GetWidth() - PROPERTIES_WIDTH, 0);
	ImVec2 panelSize = ImVec2(PROPERTIES_WIDTH, window.GetHeight());

	ImGui::SetWindowPos(panelPos);
	ImGui::SetWindowSize(panelSize);

	if (dynamic_cast<test::TestMenu*>(test) == nullptr)
		test->OnImGuiRender();

	ImGui::End();
}

void RenderDemoList(Window& window, test::TestMenu*& testMenu)
{
	ImGui::Begin("Demo List", nullptr, 
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImVec2 panelPos = ImVec2(0, 0);
	ImVec2 panelSize = ImVec2(DEMO_SELECTION_WIDTH, window.GetHeight());

	ImGui::SetWindowPos(panelPos);
	ImGui::SetWindowSize(panelSize);

	// Draw selectable scenes
	testMenu->OnImGuiRender();

	ImGui::End();
}
