#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <functional>
#include "window/Window.h"

class Scene
{
public:
	Scene(Window* window): m_Window(window) {}
	virtual ~Scene() = 0 {}

	virtual void Update(float dt) {}
	virtual void Render() {}
	virtual void RenderUI() {}

	virtual void Enter() {}
	virtual void Exit() {}
protected:
	Window* m_Window;
};

class SceneManager
{
public:
	SceneManager(Scene* currentScene);

	void ChangeScene(Scene* newScene);
	inline Scene* GetCurrentScene() const { return m_CurrentScene; }
private:
	Scene* m_CurrentScene;
};
