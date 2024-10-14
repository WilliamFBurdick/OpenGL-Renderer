#include "LightingScene.h"
#include <imgui/imgui.h>
#include "Utils.h"
#include "input/Input.h"

LightingScene::LightingScene(Window* window):
	Scene(window)
{

}

void LightingScene::Enter()
{
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	m_PhongShader = new Shader("shaders/lit/phong.vert", "shaders/lit/phong.frag");
	m_BlinnPhongShader = new Shader("shaders/lit/blinn_phong.vert", "shaders/lit/blinn_phong.frag");
	m_LightShader = new Shader("shaders/unlit/color.vert", "shaders/unlit/color.frag");

	m_DrawLights = true;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	m_CubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Set up Arrays/Buffers
	glGenVertexArrays(1, &m_CubeVAO);
	glGenBuffers(1, &m_CubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindVertexArray(m_CubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Set up light Array
	glGenVertexArrays(1, &m_LightVAO);
	glBindVertexArray(m_LightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up textures
	m_DiffuseMap = Utils::LoadTexture("res/textures/container2.png");
	m_SpecularMap = Utils::LoadTexture("res/textures/container2_specular.png");

	m_PhongShader->use();
	m_PhongShader->setInt("material.diffuse", 0);
	m_PhongShader->setInt("material.specular", 1);

	m_BlinnPhongShader->use();
	m_BlinnPhongShader->setInt("material.diffuse", 0);
	m_BlinnPhongShader->setInt("material.specular", 1);

	SetupLights();
}

void LightingScene::Exit()
{
	glDeleteBuffers(1, &m_CubeVBO);
	glDeleteVertexArrays(1, &m_CubeVAO);
	glDeleteVertexArrays(1, &m_LightVAO);
}

void LightingScene::Update(float dt)
{
	if (Input::GetMouseButton(m_Window->GetWindow(), GLFW_MOUSE_BUTTON_RIGHT))
	{
		Input::SetCursorVisible(m_Window->GetWindow(), false);
		m_Camera.ProcessMouseMovement(Input::GetMouse().xDelta, Input::GetMouse().yDelta);
		if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(EForward, dt);
		if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(EBackward, dt);
		if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(ELeft, dt);
		if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(ERight, dt);
	}
	else
	{
		Input::SetCursorVisible(m_Window->GetWindow(), true);
	}
}

void LightingScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	SetupLights();
	Shader* shader = (m_CurrentShader == 0) ? m_PhongShader : m_BlinnPhongShader;
	shader->use();
	shader->setVec3("viewPos", m_Camera.Position);
	shader->setFloat("material.shininess", 32.0f);
	ApplyLighting(shader);

	// set up view/projection matrices
	glm::mat4 view = m_Camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(m_Camera.Zoom, (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);

	// set up textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_SpecularMap);

	shader->setMat4("view", view);
	shader->setMat4("projection", proj);

	// draw scene objects
	glBindVertexArray(m_CubeVAO);
	glCheckError();
	glm::mat4 model = glm::mat4(1.0f);
	unsigned int i = 0;
	for (glm::vec3 position : m_CubePositions)
	{
		model = glm::translate(glm::mat4(1.0f), position);
		float angle = 20.0f * i++;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	if (m_DrawLights)
	{
		// Bind shader
		m_LightShader->use();
		m_LightShader->setVec4("color", glm::vec4(1.0f));
		m_LightShader->setMat4("view", view);
		m_LightShader->setMat4("projection", proj);
		// Bind light VAO
		glBindVertexArray(m_LightVAO);
		// Draw point light
		model = glm::translate(glm::mat4(1.0f), m_PointLight.position);
		model = glm::scale(model, glm::vec3(0.2f));
		m_LightShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void LightingScene::RenderUI()
{
	ImGui::RadioButton("Phong Lighting", &m_CurrentShader, 0);
	ImGui::RadioButton("Blinn-Phong Lighting", &m_CurrentShader, 1);
	ImGui::Checkbox("Draw Lights", &m_DrawLights);
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