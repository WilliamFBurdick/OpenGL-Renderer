#include "Scene.h"
#include <imgui/imgui.h>

SceneManager::SceneManager(Scene* currentScene)
{
	if (currentScene)
		currentScene->Enter();
	m_CurrentScene = currentScene;
}

void SceneManager::ChangeScene(Scene* newScene)
{
	if (m_CurrentScene)
		m_CurrentScene->Exit();
	m_CurrentScene = newScene;
	if (m_CurrentScene)
		m_CurrentScene->Enter();
}
