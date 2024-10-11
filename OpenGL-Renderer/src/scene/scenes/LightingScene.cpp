#include "LightingScene.h"
#include <imgui/imgui.h>

LightingScene::LightingScene(GLFWwindow* window):
	Scene(window)
{
	m_PhongShader = new Shader("shaders/lit/phong.vert", "shaders/lit/phong.frag");
	m_BlinnPhongShader = new Shader("shaders/lit/blinn_phong.vert", "shaders/lit/blinn_phong.frag");

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void LightingScene::Update(float dt)
{
}

void LightingScene::Render()
{
	SetupLights();
	Shader* shader = (m_CurrentShader == 0) ? m_PhongShader : m_BlinnPhongShader;
	ApplyLighting(shader);
	glClear(GL_COLOR_BUFFER_BIT);
}

void LightingScene::RenderUI()
{
	ImGui::RadioButton("Phong Lighting", &m_CurrentShader, 0);
	ImGui::RadioButton("Blinn-Phong Lighting", &m_CurrentShader, 1);
}

// Set light variables
void LightingScene::SetupLights()
{
	// Set up directional light
	m_DirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	m_DirLight.ambient = glm::vec3(0.05f);
	m_DirLight.diffuse = glm::vec3(0.4f);
	m_DirLight.specular = glm::vec3(0.5f);

	// Set up point light
	m_PointLight.position = glm::vec3(0.7f, 0.2f, 2.0f);
	m_PointLight.ambient = glm::vec3(0.05f);
	m_PointLight.diffuse = glm::vec3(0.8f);
	m_PointLight.specular = glm::vec3(1.0f);
	m_PointLight.attenuation.constant = 1.0f;
	m_PointLight.attenuation.linear = 0.09f;
	m_PointLight.attenuation.quadratic = 0.032f;

	// Set up spot light
	m_SpotLight.position = m_Camera.Position;
	m_SpotLight.direction = m_Camera.Front;
	m_SpotLight.ambient = glm::vec3(0.0f);
	m_SpotLight.diffuse = glm::vec3(1.0f);
	m_SpotLight.specular = glm::vec3(1.0f);
	m_SpotLight.attenuation.constant = 1.0f;
	m_SpotLight.attenuation.linear = 0.09f;
	m_SpotLight.attenuation.quadratic = 0.032f;
	m_SpotLight.cutOff = glm::cos(glm::radians(12.5f));
	m_SpotLight.outerCutOff = glm::cos(glm::radians(15.0f));
}

// Apply light settings to the shader
void LightingScene::ApplyLighting(Shader*& shader)
{
	shader->setInt("numDirectionalLights", 1);
	shader->setInt("numPointLights", 1);
	shader->setInt("numSpotLights", 1);
	Light::ApplyDirectionalLight(shader, m_DirLight);
	Light::ApplyPointLight(shader, m_PointLight);
	Light::ApplySpotLight(shader, m_SpotLight);
}