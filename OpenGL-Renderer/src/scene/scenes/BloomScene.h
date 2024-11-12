#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"

class BloomScene : public Scene
{
public:
	BloomScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	void RenderCube();
	void RenderQuad();
private:
	Camera m_Camera;

	// Buffers
	unsigned int m_CubeVAO, m_CubeVBO;
	unsigned int m_QuadVAO, m_QuadVBO;

	unsigned int m_hdrFBO;
	unsigned int m_ColorBuffers[2];
	unsigned int m_FBOAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	unsigned int m_DepthRBO;

	unsigned int m_PingPongFBO[2];
	unsigned int m_PingPongColorBuffers[2];

	Shader* m_StandardShader;
	Shader* m_BlurShader;
	Shader* m_FinalShader;
	Shader* m_LightShader;

	unsigned int m_WoodTexture, m_ContainerTexture;
	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;

	bool m_Bloom;
	float m_Exposure;
};

