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
protected:
	Window* m_Window;
};

class SceneMenu : public Scene
{
public:
	SceneMenu(Window* window, Scene*& currentScene);

	void RenderUI() override;
	void Render() override;

	template <typename T>
	void RegisterScene(const std::string& name)
	{
		printf("Registering scene: %s\n", name.c_str());
		m_Scenes.push_back(std::make_pair(name, [](Window* window) {return new T(window); }));
	}
private:
	Scene*& m_CurrentScene;
	std::vector<std::pair<std::string, std::function<Scene*(Window*)>>> m_Scenes;
};
