#include "Scene.h"
#include <imgui/imgui.h>

SceneMenu::SceneMenu(GLFWwindow* window, Scene*& currentScene) :
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
