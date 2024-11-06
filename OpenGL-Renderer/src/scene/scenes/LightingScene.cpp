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
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
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

	// Set up quad
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &m_QuadVBO);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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


	// Set up shadowmapping
	m_SimpleDepthShader = new Shader("./shaders/shadows/simple-depth.vert", "./shaders/shadows/simple-depth.frag");
	m_ShadowedBlinnPhongShader = new Shader("./shaders/shadows/shadowed-blinn-phong.vert", "./shaders/shadows/shadowed-blinn-phong.frag");
	m_PointDepthShader = new Shader("./shaders/shadows/point-depth.vert", "./shaders/shadows/point-depth.frag", "./shaders/shadows/point-depth.geo");
	m_PointShadowBlinnPhongShader = new Shader("./shaders/shadows/point-shadow-blinn-phong.vert", "./shaders/shadows/point-shadow-blinn-phong.frag");

	glGenFramebuffers(1, &m_DepthMapFBO);
	glGenTextures(1, &m_DepthMapTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth map texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Framebuffer is not complete!");
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// create depth cubemap
	glGenFramebuffers(1, &m_PointDepthMapFBO);
	glGenTextures(1, &m_PointDepthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// set up cubemap framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_PointDepthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_PointDepthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_ShadowedBlinnPhongShader->use();
	m_ShadowedBlinnPhongShader->setInt("diffuseMap", 0);
	m_ShadowedBlinnPhongShader->setInt("shadowMap", 1);

	m_PointShadowBlinnPhongShader->use();
	m_PointShadowBlinnPhongShader->setInt("diffuseTexture", 0);
	m_PointShadowBlinnPhongShader->setInt("depthMap", 1);
}

void LightingScene::Exit()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &m_PointDepthMapFBO);
	glDeleteFramebuffers(1, &m_DepthMapFBO);
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

	// FIRST PASS - Render depth of scene (from light's perspective) to a texture
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -1.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(m_DirLight.direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	m_SimpleDepthShader->use();
	m_SimpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCheckError();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	RenderScene(m_SimpleDepthShader);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render to Depth Cubemap (for point lights)
	lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(lightProjection * glm::lookAt(m_PointLight.position, m_PointLight.position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(m_PointLight.position, m_PointLight.position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(m_PointLight.position, m_PointLight.position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(m_PointLight.position, m_PointLight.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(m_PointLight.position, m_PointLight.position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(lightProjection * glm::lookAt(m_PointLight.position, m_PointLight.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	glBindFramebuffer(GL_FRAMEBUFFER, m_PointDepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_PointDepthShader->use();
	for (unsigned int i = 0; i < 6; ++i)
		m_PointDepthShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	m_PointDepthShader->setFloat("far_plane", far_plane);
	m_PointDepthShader->setVec3("lightPos", m_PointLight.position);
	RenderScene(m_PointDepthShader);
	glCheckError();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCheckError();

	Shader* shader;
	switch (m_CurrentShader)
	{
	case 0:
		shader = m_PhongShader;
		break;
	case 1:
		shader = m_BlinnPhongShader;
		break;
	case 2:
		shader = m_ShadowedBlinnPhongShader;
		break;
	case 3:
		shader = m_PointShadowBlinnPhongShader;
		break;
	default:
		shader = m_BlinnPhongShader;
		break;
	}

	if (m_CurrentShader < 2)
	{
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
		m_LightShader->use();
		m_LightShader->setMat4("view", view);
		m_LightShader->setMat4("projection", proj);
		shader->use();
		glCheckError();
		RenderScene(shader);
	}
	else if (m_CurrentShader == 2)
	{

		// set up view/projection matrices
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(m_Camera.Zoom, (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);
		m_LightShader->use();
		m_LightShader->setMat4("view", view);
		m_LightShader->setMat4("projection", proj);
		shader->use();
		shader->setMat4("view", view);
		shader->setMat4("projection", proj);
		shader->setVec3("viewPos", m_Camera.Position);
		shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader->setVec3("lightPos", m_DirLight.direction);
		glCheckError();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_DepthMapTexture);
		RenderScene(shader);
		glCheckError();
	}
	else if (m_CurrentShader == 3)
	{
		// set up view/projection matrices
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(m_Camera.Zoom, (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);
		m_LightShader->use();
		m_LightShader->setMat4("view", view);
		m_LightShader->setMat4("projection", proj);
		shader->use();
		shader->setMat4("view", view);
		shader->setMat4("projection", proj);
		shader->setVec3("viewPos", m_Camera.Position);
		shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shader->setVec3("lightPos", m_PointLight.position);
		shader->setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_PointDepthCubemap);
		RenderScene(shader);
		glCheckError();
	}
}

void LightingScene::RenderUI()
{
	ImGui::RadioButton("Phong Lighting", &m_CurrentShader, 0);
	ImGui::RadioButton("Blinn-Phong Lighting", &m_CurrentShader, 1);
	ImGui::RadioButton("Blinn-Phong Lighting (with shadows)", &m_CurrentShader, 2);
	ImGui::RadioButton("Blinn-Phong Lighting (with point shadows)", &m_CurrentShader, 3);
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

void LightingScene::RenderScene(Shader*& shader)
{
	// draw scene objects
	glBindVertexArray(m_CubeVAO);
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
		// Bind light VAO
		glBindVertexArray(m_LightVAO);
		// Draw point light
		model = glm::translate(glm::mat4(1.0f), m_PointLight.position);
		model = glm::scale(model, glm::vec3(0.2f));
		m_LightShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
