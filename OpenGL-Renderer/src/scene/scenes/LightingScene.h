#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"

class LightingScene : public Scene
{
public:
	LightingScene(GLFWwindow* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
private:
	// Helper methods
	void SetupLights();
	void ApplyLighting(Shader*& shader);
private:
	Camera m_Camera;
	Shader* m_PhongShader;
	Shader* m_BlinnPhongShader;
	bool m_DrawLights;

	DirectionalLight m_DirLight;
	PointLight m_PointLight;
	SpotLight m_SpotLight;

	int m_CurrentShader = 0;
};

