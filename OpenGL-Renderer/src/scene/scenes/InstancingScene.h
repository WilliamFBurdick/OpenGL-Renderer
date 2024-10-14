#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"

class InstancingScene : public Scene
{
public:
	InstancingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods

private:
	Camera m_Camera;

};