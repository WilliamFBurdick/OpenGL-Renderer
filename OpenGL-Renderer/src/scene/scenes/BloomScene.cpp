#include "BloomScene.h"
#include <imgui/imgui.h>
#include "input/Input.h"

BloomScene::BloomScene(Window* window):
	Scene(window)
{
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	m_CubeVAO = m_QuadVAO = m_CubeVBO = m_QuadVBO = 0;

	m_StandardShader = new Shader("./shaders/bloom/lit.vs", "./shaders/bloom/lit.fs");
	m_BlurShader = new Shader("./shaders/bloom/gaussian-blur.vs", "./shaders/bloom/gaussian-blur.fs");
	m_FinalShader = new Shader("./shaders/bloom/final-bloom.vs", "./shaders/bloom/final-bloom.fs");
	m_LightShader = new Shader("./shaders/bloom/lit.vs", "./shaders/bloom/bloom-light.fs");

	m_WoodTexture = Utils::LoadTexture("./res/textures/wood.png", true);
	m_ContainerTexture = Utils::LoadTexture("./res/textures/container2.png", true);

	glCheckError();

	// Create framebuffer for HDR
	glGenFramebuffers(1, &m_hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glGenTextures(2, m_ColorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColorBuffers[i], 0);
	}
	glGenRenderbuffers(1, &m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Window->GetWidth(), m_Window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
	// Tell OpenGL which color attachments we'll use (in this framebuffer) for rendering
	glDrawBuffers(2, m_FBOAttachments);
	// Check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create framebuffers for Ping-Pong Gaussian Blur effect
	glGenFramebuffers(2, m_PingPongFBO);
	glGenTextures(2, m_PingPongColorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Window->GetWidth(), m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PingPongColorBuffers[i], 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("Framebuffer not complete!");
		}
	}

	// Set up lighting
	m_LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	m_LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	m_LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	m_LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));

	m_LightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
	m_LightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
	m_LightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

	// Configure shaders
	m_StandardShader->use();
	m_StandardShader->setInt("diffuseTexture", 0);
	m_BlurShader->use();
	m_BlurShader->setInt("image", 0);
	m_FinalShader->use();
	m_FinalShader->setInt("scene", 0);
	m_FinalShader->setInt("bloomBlur", 1);

	m_Bloom = true;
	m_Exposure = 0.2f;
}

void BloomScene::Update(float dt)
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

void BloomScene::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);
	glm::mat4 view = m_Camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	m_StandardShader->use();
	m_StandardShader->setMat4("projection", projection);
	m_StandardShader->setMat4("view", view);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_WoodTexture);
	// set lighting
	for (unsigned int i = 0; i < m_LightPositions.size(); i++)
	{
		m_StandardShader->setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
		m_StandardShader->setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
	}
	m_StandardShader->setVec3("viewPos", m_Camera.Position);
	// large cube (floor)
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
	m_StandardShader->setMat4("model", model);
	RenderCube();
	// multiple cubes for scenery
	glBindTexture(GL_TEXTURE_2D, m_ContainerTexture);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	m_StandardShader->setMat4("model", model);
	RenderCube();
	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	m_StandardShader->setMat4("model", model);
	RenderCube();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 2.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	m_StandardShader->setMat4("model", model);
	RenderCube();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.7f, 4.0f));
	model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	model = glm::scale(model, glm::vec3(1.25f));
	m_StandardShader->setMat4("model", model);
	RenderCube();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, -3.0f));
	model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	m_StandardShader->setMat4("model", model);
	RenderCube();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	m_StandardShader->setMat4("model", model);
	RenderCube();

	// Draw light sources
	m_LightShader->use();
	m_LightShader->setMat4("projection", projection);
	m_LightShader->setMat4("view", view);
	for (unsigned int i = 0; i < m_LightPositions.size(); i++)
	{
		model = glm::translate(glm::mat4(1.0f), glm::vec3(m_LightPositions[i]));
		model = glm::scale(model, glm::vec3(0.25f));
		m_LightShader->setMat4("model", model);
		m_LightShader->setVec3("lightColor", m_LightColors[i]);
		RenderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Blur the bright fragments with two-pass Gaussian Blur
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	m_BlurShader->use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PingPongFBO[horizontal]);
		m_BlurShader->setInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? m_ColorBuffers[1] : m_PingPongColorBuffers[!horizontal]);
		RenderQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Now render floating point color buffer to 2D quad and tonemap HDR colors
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_FinalShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_PingPongColorBuffers[!horizontal]);
	m_FinalShader->setInt("bloom", m_Bloom);
	m_FinalShader->setFloat("exposure", m_Exposure);
	RenderQuad();
}

void BloomScene::RenderUI()
{
	ImGui::Checkbox("Bloom enabled?", &m_Bloom);
	ImGui::DragFloat("Exposure", &m_Exposure, 0.01f, 0.0f, 5.0f);
}

void BloomScene::Enter()
{
}

void BloomScene::Exit()
{
}

void BloomScene::RenderCube()
{
	if (m_CubeVAO == 0)
	{
		// Create Cube
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &m_CubeVAO);
		glGenBuffers(1, &m_CubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(m_CubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void BloomScene::RenderQuad()
{
	if (m_QuadVAO == 0)
	{
		// Create quad
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &m_QuadVAO);
		glGenBuffers(1, &m_QuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
		glBindVertexArray(m_QuadVAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(m_QuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
