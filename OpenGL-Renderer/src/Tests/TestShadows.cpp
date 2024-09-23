#include "TestShadows.h"
#include "Input/Input.h"
#include <imgui/imgui.h>

void RenderQuad();
void RenderCube();

test::TestShadows::TestShadows(Window* window):
	Test(window),
	mSimpleDepth("res/shaders/simpledepth.vert", "res/shaders/simpledepth.frag"),
	mObjectShader("res/shaders/shadowmapping.vert", "res/shaders/shadowmapping.frag"),
	mDebugQuad("res/shaders/debug_quad.vert", "res/shaders/debug_quad.frag"),
	mWood("res/textures/wood.png")
{
	// Reset OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	mCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	// ==========================================

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	mPlaneVBO = VBO(planeVertices, sizeof(planeVertices));
	mPlaneVAO.AddAttribute(mPlaneVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	mPlaneVAO.AddAttribute(mPlaneVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	mPlaneVAO.AddAttribute(mPlaneVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	mPlaneVAO.Unbind();

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	mQuadVBO = VBO(quadVertices, sizeof(quadVertices));
	mQuadVAO.AddAttribute(mQuadVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	mQuadVAO.AddAttribute(mQuadVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// Create depth map fbo
	glGenFramebuffers(1, &mDepthMapFBO);
	// create depth texture
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mDebugQuad.use();
	mDebugQuad.SetInt("depthMap", 0);

	// Set up light
	mLightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
}

test::TestShadows::~TestShadows()
{
}

void test::TestShadows::OnUpdate(float deltaTime)
{
	if (Input::GetMouseButton(mWindow->GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		mCamera.ProcessMouseMovement(Input::Cursor.xDelta, Input::Cursor.yDelta);
		if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
			mCamera.ProcessKeyboard(EForward, deltaTime);
		if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
			mCamera.ProcessKeyboard(EBackward, deltaTime);
		if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
			mCamera.ProcessKeyboard(ELeft, deltaTime);
		if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
			mCamera.ProcessKeyboard(ERight, deltaTime);
		if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
			mCamera.ProcessKeyboard(EUp, deltaTime);
		if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			mCamera.ProcessKeyboard(EDown, deltaTime);

		Input::SetMouseVisible(mWindow->GetWindow(), false);
	}
	else
	{
		Input::SetMouseVisible(mWindow->GetWindow(), true);
	}
}

void test::TestShadows::OnRender()
{
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(mLightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	mSimpleDepth.use();
	mSimpleDepth.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, 2048, 2048);
	glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	mWood.Bind(0);
	RenderScene(mSimpleDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	int x, y, w, h;
	mWindow->GetViewport(x, y, w, h);
	glViewport(x, y, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mObjectShader.use();
	glm::mat4 projection = glm::perspective(glm::radians(mCamera.Zoom), (float)w / (float)h, 0.1f, 100.0f);
	glm::mat4 view = mCamera.GetViewMatrix();
	mObjectShader.SetMat4("projection", projection);
	mObjectShader.SetMat4("view", view);
	// set light uniforms
	mObjectShader.SetVec3("viewPos", mCamera.Position);
	mObjectShader.SetVec3("lightPos", mLightPos);
	mObjectShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	mWood.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	RenderScene(mObjectShader);
}

void test::TestShadows::OnImGuiRender()
{
	ImGui::Text("Shadowmapping");
}

unsigned int test::TestShadows::GenerateShadowmap(int width, int height)
{
	mShadowWidth = width;
	mShadowHeight = height;
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return depthMap;
}

void test::TestShadows::RenderScene(const Shader& shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.SetMat4("model", model);
	mPlaneVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	RenderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetMat4("model", model);
	RenderCube();
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

unsigned int shadowCubeVAO = 0;
unsigned int shadowCubeVBO = 0;
void RenderCube()
{
	// initialize (if necessary)
	if (shadowCubeVAO == 0)
	{
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
		glGenVertexArrays(1, &shadowCubeVAO);
		glGenBuffers(1, &shadowCubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, shadowCubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(shadowCubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(shadowCubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}