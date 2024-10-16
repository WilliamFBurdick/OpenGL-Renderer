#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "camera/Camera.h"

enum PostProcessingEffect
{
	NONE = 0, INVERT = 1, GREYSCALE = 2, SHARPEN = 3, BLUR = 4, EDGE_DETECT = 5
};

class PostProcessingScene : public Scene
{
public:
	PostProcessingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
	void GenerateFramebuffer();
private:
	Camera m_Camera;

	Shader* m_UnlitShader;
	Shader* m_PostProcessingShader;

	PostProcessingEffect m_CurrentEffect;

	// Framebuffer objects
	unsigned int m_FBO, m_RBO;
	unsigned int m_ScreenTexture;

	// Scene objects
	unsigned int m_CubeVAO;
	unsigned int m_CubeVBO;

	unsigned int m_PlaneVAO;
	unsigned int m_PlaneVBO;

	unsigned int m_QuadVAO;
	unsigned int m_QuadVBO;

	// Textures
	unsigned int m_CubeTexture;
	unsigned int m_FloorTexture;
};

