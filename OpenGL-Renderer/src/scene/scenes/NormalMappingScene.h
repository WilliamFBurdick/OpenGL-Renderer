#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"
#include "model/Model.h"

class NormalMappingScene : public Scene
{
public:
	NormalMappingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods

private:
	// Members
	Camera m_Camera;
	// Buffers
	unsigned int m_QuadVAO;
	unsigned int m_QuadVBO;
	// Textures
	unsigned int m_QuadDiffuseMap, m_QuadNormalMap;
	unsigned int m_BricksDiffuse, m_BricksNormal, m_BricksHeight;

	// Shaders
	Shader* m_NormalMapShader;
	Shader* m_ParallaxMapShader;
	int m_ActiveShader;

	// Lights
	glm::vec3 m_LightPos;

	// Height scaling
	float m_HeightScale;
};

