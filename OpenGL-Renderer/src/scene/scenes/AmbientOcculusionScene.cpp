#include "AmbientOcculusionScene.h"
#include <random>
#include "input/Input.h"
#include <imgui/imgui.h>

AmbientOcculusionScene::AmbientOcculusionScene(Window* window):
	Scene(window)
{
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));

	m_ShaderGeometryPass = new Shader("./shaders/ambient-occlusion/ssao-geometry.vs", "./shaders/ambient-occlusion/ssao-geometry.fs");
	m_ShaderLightingPass = new Shader("./shaders/ambient-occlusion/ssao.vs", "./shaders/ambient-occlusion/ssao-lighting.fs");
	m_ShaderSSAO = new Shader("./shaders/ambient-occlusion/ssao.vs", "./shaders/ambient-occlusion/ssao.fs");
	m_ShaderSSAOBlur = new Shader("./shaders/ambient-occlusion/ssao.vs", "./shaders/ambient-occlusion/ssao-blur.fs");

	m_Backpack = new Model("./res/models/backpack/backpack.obj");

	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// position color buffer
	glGenTextures(1, &m_gPosition);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);
	// normal color buffer
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &m_gAlbedo);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedo, 0);
	// tell OpenGL which color attachments we're using in this framebuffer
	glDrawBuffers(3, m_Attachments);

	// create and attach depth buffer
	glGenRenderbuffers(1, &m_DepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (float)m_Window->GetWidth(), (float)m_Window->GetHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// create framebuffer to hold SSAO processing stage
	glGenFramebuffers(1, &m_ssaoFBO);
	glGenFramebuffers(1, &m_ssaoBlurFBO);
	// SSAO color buffer
	glGenTextures(1, &m_ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("SSAO Framebuffer not complete!");
	}
	// and blur stage
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoColorBufferBlur);
	glGenTextures(1, &m_ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("SSAO Blur Framebuffer not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// generate sample kernel
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; i++)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;
		// scale samples so they're more aligned to center of kernel
		scale = Lerp(0.1f, 1.0f, scale * scale);
		sample *= sample;
		m_ssaoKernel.push_back(sample);
	}

	// generate noise texture
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);	// rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &m_noiseTexture);
	glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// lighting info
	m_LightPos = glm::vec3(2.0f, 4.0f, -2.0f);
	m_LightColor = glm::vec3(0.2f, 0.2f, 0.7f);

	// shader configuration
	m_ShaderLightingPass->use();
	m_ShaderLightingPass->setInt("gPosition", 0);
	m_ShaderLightingPass->setInt("gNormal", 1);
	m_ShaderLightingPass->setInt("gAlbedo", 2);
	m_ShaderLightingPass->setInt("ssao", 3);
	m_ShaderSSAO->use();
	m_ShaderSSAO->setInt("gPosition", 0);
	m_ShaderSSAO->setInt("gNormal", 1);
	m_ShaderSSAO->setInt("texNoise", 2);
	m_ShaderSSAOBlur->use();
	m_ShaderSSAOBlur->setInt("ssaoInput", 0);
}

void AmbientOcculusionScene::Update(float dt)
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

void AmbientOcculusionScene::Render()
{
	// Step 1. Geometry Pass - render scene's geometry/color data to gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 50.0f);
	glm::mat4 view = m_Camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	m_ShaderGeometryPass->use();
	m_ShaderGeometryPass->setMat4("projection", projection);
	m_ShaderGeometryPass->setMat4("view", view);
	// room cube
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.5f));
	m_ShaderGeometryPass->setMat4("model", model);
	m_ShaderGeometryPass->setInt("invertedNormals", 1);
	RenderCube();
	m_ShaderGeometryPass->setInt("invertedNormals", 0);
	// backpack model on the floor
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	m_ShaderGeometryPass->setMat4("model", model);
	m_Backpack->Draw(*m_ShaderGeometryPass);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Step 2. Generate SSAO texture
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ShaderSSAO->use();
	// send kernel + rotation
	for (unsigned int i = 0; i < 64; i++)
	{
		m_ShaderSSAO->setVec3("samples[" + std::to_string(i) + "]", m_ssaoKernel[i]);
	}
	m_ShaderSSAO->setMat4("projection", projection);
	m_ShaderSSAO->setInt("kernelSize", m_KernelSize);
	m_ShaderSSAO->setFloat("radius", m_Radius);
	m_ShaderSSAO->setFloat("bias", m_Bias);
	m_ShaderSSAO->setFloat("screen_width", (float)m_Window->GetWidth());
	m_ShaderSSAO->setFloat("screen_height", (float)m_Window->GetHeight());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_noiseTexture);
	RenderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Step 3. Blur SSAO texture to remove visible noise
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ShaderSSAOBlur->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColorBuffer);
	RenderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Step 4. Lighting Pass : deferred Blinn-Phong shading with screen-space ambient occlusion added
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_ShaderLightingPass->use();
	// send light uniforms
	glm::vec3 lightPosView = glm::vec3(m_Camera.GetViewMatrix() * glm::vec4(m_LightPos, 1.0));
	m_ShaderLightingPass->setVec3("light.Position", lightPosView);
	m_ShaderLightingPass->setVec3("light.Color", m_LightColor);
	// update attenuation
	const float linear = 0.09f;
	const float quadratic = 0.032f;
	m_ShaderLightingPass->setFloat("light.Linear", linear);
	m_ShaderLightingPass->setFloat("light.Quadratic", quadratic);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gAlbedo);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_ssaoColorBuffer);
	RenderQuad();
}

void AmbientOcculusionScene::RenderUI()
{
	ImGui::DragInt("Kernel Size", &m_KernelSize, 1, 32, 128);
	ImGui::DragFloat("Radius", &m_Radius, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Bias", &m_Bias, 0.001f, 0.0f, 1.0f);
}

void AmbientOcculusionScene::Enter()
{

}

void AmbientOcculusionScene::Exit()
{

}

void AmbientOcculusionScene::RenderQuad()
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

void AmbientOcculusionScene::RenderCube()
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

float AmbientOcculusionScene::Lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
