#include "TestShadows.h"
#include "Input/Input.h"

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
	float cubeVertices[] = {
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
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	mPlaneVBO = VBO(planeVertices, sizeof(planeVertices));
	mPlaneVAO.AddAttribute(mPlaneVBO, 0, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)0);
	mPlaneVAO.AddAttribute(mPlaneVBO, 1, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	mPlaneVAO.AddAttribute(mPlaneVBO, 2, 2 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	mCubeVBO = VBO(cubeVertices, sizeof(cubeVertices));
	mCubeVAO.AddAttribute(mCubeVBO, 0, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)0);
	mCubeVAO.AddAttribute(mCubeVBO, 1, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	mCubeVAO.AddAttribute(mCubeVBO, 2, 2 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	mQuadVBO = VBO(quadVertices, sizeof(quadVertices));
	mQuadVAO.AddAttribute(mQuadVBO, 0, 3 * sizeof(float), GL_FLOAT, 5 * sizeof(float), (void*)0);
	mQuadVAO.AddAttribute(mQuadVBO, 1, 2 * sizeof(float), GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// Set up Depthmap texure and Shadowmap framebuffer
	glGenFramebuffers(1, &mShadowMapFBO);
	//mDepthMap = GenerateShadowmap(2048, 2048);
	// Generate depth texture
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Attach depth map to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mObjectShader.SetInt("diffuseTexture", 0);
	mObjectShader.SetInt("shadowMap", 1);

	mCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	// Set up view/projection
	int x, y, w, h;
	mWindow->GetViewport(x, y, w, h);
	glm::mat4 proj = glm::perspective(mCamera.Zoom, (float)w / (float)h, 0.1f, 100.0f);
	glm::mat4 view = mCamera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	float nearPlane = 1.0f;
	float farPlane = 7.5f;
	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(mDirLight.direction * 20.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProj * lightView;
	// render from light's pov
	mSimpleDepth.use();
	mSimpleDepth.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, mShadowWidth, mShadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	mWood.Bind(0);
	RenderScene(mSimpleDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mObjectShader.use();
	mObjectShader.SetMat4("projection", proj);
	mObjectShader.SetMat4("view", view);
	mObjectShader.SetVec3("viewPos", mCamera.Position);
	mObjectShader.SetVec3("lightPos", mDirLight.direction * 20.0f);
	mObjectShader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
	mWood.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	RenderScene(mObjectShader);

	mDebugQuad.use();
	mDebugQuad.SetFloat("near_plane", nearPlane);
	mDebugQuad.SetFloat("far_plane", farPlane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	mQuadVAO.Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	mQuadVAO.Unbind();
}

void test::TestShadows::OnImGuiRender()
{
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
	glm::mat4 model = glm::mat4(1.0f);
	// Draw cubes
	mWood.Bind(0);
	mCubeVAO.Bind();
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0));
	mObjectShader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0));
	mObjectShader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw floor
	mWood.Bind(0);
	mPlaneVAO.Bind();
	model = glm::mat4(1.0f);
	mObjectShader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
