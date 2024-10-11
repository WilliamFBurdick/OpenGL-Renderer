#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdio>

#include "window/Window.h"
#include "input/Input.h"
#include "camera/Camera.h"
#include "model/Model.h"
#include "shader/Shader.h"
#include "scene/Scene.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "scene/scenes/LightingScene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void renderProperties(GLFWwindow* window, Scene*& scene);
void renderSceneSelection(GLFWwindow* window, SceneMenu*& sceneMenu);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int PANEL_WIDTH = 150;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_WIDTH / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{
	// create window
	Window window("OpenGL Renderer", 1280, 720);

	// configure imgui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
	ImGui::StyleColorsDark();

	// CONFIGURE GLOBAL OPENGL STATE
	glEnable(GL_DEPTH_TEST);

	// LOAD SCENES
	Scene* currentScene;
	SceneMenu* sceneMenu = new SceneMenu(&window, currentScene);
	currentScene = sceneMenu;

	sceneMenu->RegisterScene<LightingScene>("Lighting");


	// RENDER LOOP
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		// timing
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		Input::ProcessInput(window.GetWindow());
		if (Input::GetKey(window.GetWindow(), GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window.GetWindow(), true);
		}

		// rendering
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (currentScene)
		{
			currentScene->Update(deltaTime);
			currentScene->Render();

			renderProperties(window.GetWindow(), currentScene);
			renderSceneSelection(window.GetWindow(), sceneMenu);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// check and call events and swap buffers
		glfwSwapBuffers(window.GetWindow());
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void renderProperties(GLFWwindow* window, Scene*& scene)
{
	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	ImVec2 panelPos = ImVec2(w - PANEL_WIDTH, 0);
	ImVec2 panelSize = ImVec2(PANEL_WIDTH, h);

	ImGui::SetWindowPos(panelPos);
	ImGui::SetWindowSize(panelSize);

	scene->RenderUI();
	ImGui::End();
}

void renderSceneSelection(GLFWwindow* window, SceneMenu*& sceneMenu)
{
	ImGui::Begin("Scene Selection", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);

	ImVec2 panelPos = ImVec2(0, 0);
	ImVec2 panelSize = ImVec2(PANEL_WIDTH, h);

	ImGui::SetWindowPos(panelPos);
	ImGui::SetWindowSize(panelSize);

	sceneMenu->RenderUI();
	ImGui::End();
}