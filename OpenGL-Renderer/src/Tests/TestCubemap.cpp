#include "TestCubemap.h"

test::TestCubemap::TestCubemap(Window* window):
	Test(window)
{
	std::vector<const char*> skyboxTextures = {
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/back.jpg",
		"res/textures/skybox/front.jpg"
	};
	mCubemap = Texture(skyboxTextures);
}

test::TestCubemap::~TestCubemap()
{
}

void test::TestCubemap::OnUpdate(float deltaTime)
{
}

void test::TestCubemap::OnRender()
{
}

void test::TestCubemap::OnImGuiRender()
{
}
