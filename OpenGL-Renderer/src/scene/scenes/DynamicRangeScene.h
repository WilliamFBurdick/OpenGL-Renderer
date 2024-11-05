#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"
#include "model/Model.h"

class DynamicRangeScene : public Scene
{
public:
	DynamicRangeScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
private:
	Camera m_Camera;

	// Buffers
	unsigned int m_CubeVAO, m_CubeVBO;
	unsigned int m_QuadVAO, m_QuadVBO;

	unsigned int m_hdrFBO;
	unsigned int m_ColorBuffer;
	unsigned int m_DepthRBO;

	Shader* m_LightingShader;
	Shader* m_HDRShader;

	unsigned int m_WoodTexture;
	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;

	bool m_HDR;
	float m_Exposure;
};

