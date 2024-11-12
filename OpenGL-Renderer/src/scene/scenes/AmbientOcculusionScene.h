#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"
#include "model/Model.h"

class AmbientOcculusionScene : public Scene
{
public:
	AmbientOcculusionScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
	void RenderQuad();
	void RenderCube();
	float Lerp(float a, float b, float f);
private:
	// Members
	Camera m_Camera;

	Shader* m_ShaderGeometryPass;
	Shader* m_ShaderLightingPass;
	Shader* m_ShaderSSAO;
	Shader* m_ShaderSSAOBlur;

	Model* m_Backpack;

	unsigned int m_gBuffer;
	unsigned int m_gPosition, m_gNormal, m_gAlbedo;
	unsigned int m_Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int m_DepthRBO;

	unsigned int m_ssaoFBO, m_ssaoBlurFBO;
	unsigned int m_ssaoColorBuffer, m_ssaoColorBufferBlur;

	std::vector<glm::vec3> m_ssaoKernel;
	unsigned int m_noiseTexture;

	glm::vec3 m_LightPos;
	glm::vec3 m_LightColor;

	unsigned int m_CubeVAO, m_CubeVBO;
	unsigned int m_QuadVAO, m_QuadVBO;

	int m_KernelSize = 64;
	float m_Radius = 0.5;
	float m_Bias = 0.025;
};

