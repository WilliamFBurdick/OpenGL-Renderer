#include "TestShadows.h"

test::TestShadows::TestShadows(Window* window):
	Test(window),
	mSimpleDepth("res/shaders/simpledepth.vert", "res/shaders/simpledepth.frag"),
	mWood("res/textures/wood.png")
{
	// Reset OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	// Set up Depthmap texure and Shadowmap framebuffer
	glGenFramebuffers(1, &mShadowMapFBO);
	mDepthMap = GenerateShadowmap(1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

test::TestShadows::~TestShadows()
{
}

void test::TestShadows::OnUpdate(float deltaTime)
{
}

void test::TestShadows::OnRender()
{
	glViewport(0, 0, mShadowWidth, mShadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Configure shader and matrices
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	// Render scene from light's pov

	glViewport(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
	// Render scene with shadows
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
