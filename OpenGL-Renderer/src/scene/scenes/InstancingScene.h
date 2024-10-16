#pragma once

#include "scene/Scene.h"
#include "shader/Shader.h"
#include <glm/glm.hpp>
#include "lights/Lights.h"
#include "camera/Camera.h"
#include "model/Model.h"

class InstancingScene : public Scene
{
public:
	InstancingScene(Window* window);
	void Update(float dt) override;
	void Render() override;
	void RenderUI() override;
	void Enter() override;
	void Exit() override;
private:
	// Helper methods
	void GenerateModelMatrices(unsigned int count);
	void GenerateInstanceMatrix();
private:
	Camera m_Camera;

	// Shaders
	Shader* m_InstancedShader;
	Shader* m_NotInstancedShader;

	// Models
	Model* m_Planet;
	Model* m_Asteroid;
	Model* m_AsteroidInstanced;

	unsigned int m_InstanceMatrixVBO;
	bool m_InstancedRender;
	unsigned int m_AsteroidCount;
	std::vector<glm::mat4> m_ModelMatrices;
};