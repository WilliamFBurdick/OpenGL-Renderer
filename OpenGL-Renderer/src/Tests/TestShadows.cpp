#include "TestShadows.h"

test::TestShadows::TestShadows(Window* window):
	Test(window),
	mSimpleDepth("res/shaders/simpledepth.vert", "res/shaders/simpledepth.frag"),
	mObjectShader("res/shaders/shadowmapping.vert", "res/shaders/shadowmapping.frag"),
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

	mPlaneVBO = VBO(planeVertices, sizeof(planeVertices));
	mPlaneVAO.AddAttribute(mPlaneVBO, 0, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)0);
	mPlaneVAO.AddAttribute(mPlaneVBO, 1, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	mPlaneVAO.AddAttribute(mPlaneVBO, 2, 2 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	mCubeVBO = VBO(cubeVertices, sizeof(cubeVertices));
	mCubeVAO.AddAttribute(mCubeVBO, 0, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)0);
	mCubeVAO.AddAttribute(mCubeVBO, 1, 3 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	mCubeVAO.AddAttribute(mCubeVBO, 2, 2 * sizeof(float), GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Set up Depthmap texure and Shadowmap framebuffer
	glGenFramebuffers(1, &mShadowMapFBO);
	mDepthMap = GenerateShadowmap(1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mObjectShader.SetInt("diffuseTexture", 0);
	mObjectShader.SetInt("shadowMap", 1);

	mLightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
}

test::TestShadows::~TestShadows()
{
}

void test::TestShadows::OnUpdate(float deltaTime)
{
}

void test::TestShadows::OnRender()
{
	// Set up matrices

	// Configure shader and matrices
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	glViewport(0, 0, mShadowWidth, mShadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	mWood.Bind(0);
	RenderScene(mSimpleDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset viewport
	glViewport(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Render scene with shadows
	mObjectShader.use();
	glm::mat4 view = mCamera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(mCamera.Zoom, (float)mWindow->GetWidth() / (float)mWindow->GetHeight(), 0.1f, 100.0f);
	mObjectShader.SetMat4("projection", projection);
	mObjectShader.SetMat4("view", view);
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
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	// draw floor
	shader.SetMat4("model", model);
	mPlaneVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// draw cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetMat4("model", model);
	mCubeVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
	model = glm::scale(model, glm::vec3(0.25f));
	shader.SetMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
