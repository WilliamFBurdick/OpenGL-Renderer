#include "Scene.h"
#include <imgui/imgui.h>

SceneMenu::SceneMenu(Window* window, Scene*& currentScene) :
	Scene(window), m_CurrentScene(currentScene)
{

}

void SceneMenu::RenderUI()
{
	for (auto& scene : m_Scenes)
	{
		if (ImGui::Button(scene.first.c_str()))
		{
			m_CurrentScene = scene.second(m_Window);
		}
	}
}

void SceneMenu::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
