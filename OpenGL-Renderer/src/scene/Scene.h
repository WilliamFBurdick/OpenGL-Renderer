#pragma once
#include <vector>
#include <string>
#include <functional>

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Update(float dt) {}
	virtual void Render() {}
	virtual void RenderUI() {}
};

class SceneMenu : public Scene
{
public:
	SceneMenu();
	~SceneMenu();

	void RenderUI() override;
private:
	Scene* m_CurrentScene;
	std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
};
