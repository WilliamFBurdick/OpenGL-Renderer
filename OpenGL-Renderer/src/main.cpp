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
#include "scene/scenes/InstancingScene.h"
#include "scene/scenes/PostProcessingScene.h"
#include "scene/scenes/AlphaBlendingScene.h"

void renderProperties(Window* window, Scene* scene);
void renderSceneSelection(Window* window, SceneManager& sceneManager);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int PANEL_WIDTH = 250;

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
	SceneManager sceneManager(new LightingScene(&window));


	// RENDER LOOP
	while (!glfwWindowShouldClose(window.GetWindow()))
	{
		// timing
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

		if (sceneManager.GetCurrentScene())
		{
			sceneManager.GetCurrentScene()->Update(deltaTime);
			sceneManager.GetCurrentScene()->Render();

			renderProperties(&window, sceneManager.GetCurrentScene());
			renderSceneSelection(&window, sceneManager);
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

void renderProperties(Window* window, Scene* scene)
{
	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	ImVec2 panelPos = ImVec2(window->GetWidth() - PANEL_WIDTH, 0);
	ImVec2 panelSize = ImVec2(PANEL_WIDTH, window->GetHeight());

	ImGui::SetWindowPos(panelPos);
	ImGui::SetWindowSize(panelSize);

	scene->RenderUI();
	ImGui::End();
}

void renderSceneSelection(Window* window, SceneManager& sceneManager)
{
	ImGui::Begin("Scene Selection", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	ImVec2 panelPos = ImVec2(0, 0);
	ImVec2 panelSize = ImVec2(PANEL_WIDTH, window->GetHeight());

	ImGui::SetWindowPos(panelPos);
	ImGui::SetWindowSize(panelSize);

	ImGui::Text("Select a scene to view:");
	// Render Scene Selection
	if (ImGui::Button("Lighting"))
		sceneManager.ChangeScene(new LightingScene(window));
	if (ImGui::Button("Instancing"))
		sceneManager.ChangeScene(new InstancingScene(window));
	if (ImGui::Button("Post Processing"))
		sceneManager.ChangeScene(new PostProcessingScene(window));
	if (ImGui::Button("Alpha Blending"))
		sceneManager.ChangeScene(new AlphaBlendingScene(window));

	ImGui::End();
}