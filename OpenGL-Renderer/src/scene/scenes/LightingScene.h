#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"

class LightingScene : public Scene
{
public:
	LightingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
	void SetupLights();
	void ApplyLighting(Shader*& shader);
	void RenderScene(Shader*& shader);
private:
	Camera m_Camera;
	Shader* m_PhongShader;
	Shader* m_BlinnPhongShader;
	Shader* m_LightShader;
	bool m_DrawLights;

	DirectionalLight m_DirLight;
	PointLight m_PointLight;
	SpotLight m_SpotLight;

	int m_CurrentShader = 0;

	// Scene Objects
	unsigned int m_CubeVAO;
	unsigned int m_CubeVBO;
	unsigned int m_LightVAO;
	std::vector<glm::vec3> m_CubePositions;
	// Textures
	unsigned int m_DiffuseMap;
	unsigned int m_SpecularMap;

	// Shadowmapping
	unsigned int m_QuadVAO;
	unsigned int m_QuadVBO;
	unsigned int m_DepthMapFBO;
	unsigned int m_DepthMapTexture;
	unsigned int m_PointDepthMapFBO;
	unsigned int m_PointDepthCubemap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	Shader* m_SimpleDepthShader;
	Shader* m_PointDepthShader;
	Shader* m_ShadowedBlinnPhongShader;
	Shader* m_PointShadowBlinnPhongShader;
};

