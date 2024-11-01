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

	// Shaders
	Shader* m_NormalMapShader;

	// Lights
	glm::vec3 m_LightPos;
};

