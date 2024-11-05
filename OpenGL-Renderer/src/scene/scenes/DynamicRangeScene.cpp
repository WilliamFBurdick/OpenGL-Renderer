#include "DynamicRangeScene.h"
#include <imgui/imgui.h>
#include "input/Input.h"

DynamicRangeScene::DynamicRangeScene(Window* window):
	Scene(window)
{
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));

	m_LightingShader = new Shader("./shaders/hdr/lit.vs", "./shaders/hdr/lit.fs");
	m_HDRShader = new Shader("./shaders/hdr/hdr.vs", "./shaders/hdr/hdr.fs");

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_WoodTexture = Utils::LoadTexture("./res/textures/wood.png", true);
	glGenFramebuffers(1, &m_hdrFBO);
	// Create floating point color buffer
	glGenTextures(1, &m_ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Window->GetWidth(), m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Create depth RBO
	glGenRenderbuffers(1, &m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Window->GetWidth(), m_Window->GetHeight());
	// Attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_LightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	m_LightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	m_LightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	m_LightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
	// colors
	m_LightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	m_LightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
	m_LightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

	m_LightingShader->use();
	m_LightingShader->setInt("diffuseTexture", 0);
	m_HDRShader->use();
	m_HDRShader->setInt("hdrBuffer", 0);

	m_HDR = false;
	m_Exposure = 0.5f;
}

void DynamicRangeScene::Update(float dt)
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

void DynamicRangeScene::Render()
{
	// Render scene to floating point framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (GLfloat)m_Window->GetWidth() / (GLfloat)m_Window->GetHeight(), 0.1f, 100.0f);
	glm::mat4 view = m_Camera.GetViewMatrix();
	m_LightingShader->use();
	m_LightingShader->setMat4("projection", projection);
	m_LightingShader->setMat4("view", view);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_WoodTexture);
	// set lighting uniforms
	for (unsigned int i = 0; i < m_LightPositions.size(); i++)
	{
		m_LightingShader->setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
		m_LightingShader->setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
	}
	m_LightingShader->setVec3("viewPos", m_Camera.Position);
	// render tunnel
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f));
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
	m_LightingShader->setMat4("model", model);
	m_LightingShader->setInt("inverse_normals", true);
	// render cube
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Now render floating point color buffer to 2D quad and tonemap HDR colors
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_HDRShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
	m_HDRShader->setInt("hdr", m_HDR);
	m_HDRShader->setFloat("exposure", m_Exposure);
	glBindVertexArray(m_QuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DynamicRangeScene::RenderUI()
{
	ImGui::Checkbox("HDR enabled?", &m_HDR);
	ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 100.0f);
}

void DynamicRangeScene::Enter()
{

}

void DynamicRangeScene::Exit()
{

}
