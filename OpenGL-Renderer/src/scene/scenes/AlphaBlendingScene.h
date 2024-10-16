#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"

class AlphaBlendingScene : public Scene
{
public:
	AlphaBlendingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
private:
	Camera m_Camera;
	Shader* m_UnlitShader;

	bool m_Sort;

	// Scene objects
	unsigned int m_CubeVAO, m_CubeVBO;
	unsigned int m_PlaneVAO, m_PlaneVBO;
	unsigned int m_TransparentVAO, m_TransparentVBO;
	// Textures
	unsigned int m_CubeTexture, m_FloorTexture, m_TransparentTexture;

	std::vector<glm::vec3> m_WindowPositions;
	float m_Runtime;
};

