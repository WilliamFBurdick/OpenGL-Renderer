#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"
#include "model/Model.h"

class DeferredShadingScene : public Scene
{
public:
	DeferredShadingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
	void RenderQuad();
	void RenderCube();
private:
	// Members
	Camera m_Camera;

	unsigned int m_QuadVAO, m_QuadVBO;
	unsigned int m_CubeVAO, m_CubeVBO;

	std::vector<glm::vec3> m_ObjectPositions;
	unsigned int m_GBuffer;
	unsigned int m_GPosition, m_GNormal, m_GAlbedoSpec;
	unsigned int m_Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int m_DepthRBO;

	const unsigned int NR_LIGHTS = 32;
	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;

	Model* m_Backpack;

	Shader* m_GeometryPassShader;
	Shader* m_LightingPassShader;
	Shader* m_LightShader;
};

