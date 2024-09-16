#include "TestShadows.h"

test::TestShadows::TestShadows(Window* window):
	Test(window)
{
	// Reset OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
}

void test::TestShadows::OnImGuiRender()
{
}

unsigned int test::TestShadows::GenerateShadowmap(int width, int height)
{
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return depthMap;
}
