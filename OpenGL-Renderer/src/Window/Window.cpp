#include "Window.h"
#include <cstdio>

Window::Window(int width, int height, const char* title):
	mWidth(width), mHeight(height), mTitle(title)
{
	mWindow = nullptr;
}

Window::~Window()
{
	glfwDestroyWindow(mWindow);
}

bool Window::Init()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Set swap interval to 1:1
	glfwSwapInterval(1);

	// Enable multisampling
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create GLFW window
	mWindow = glfwCreateWindow(mWidth, mHeight, mTitle, NULL, NULL);
	if (!mWindow)
	{
		printf("[GLFW Error]: Window: Failed to create GLFW window!\n");
		return false;
	}
	glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, this);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("[GLAD Error]: Window: Failed to initialize GLAD!\n");
		return false;
	}

	glViewport(0, 0, mWidth, mHeight);

	glfwSetFramebufferSizeCallback(mWindow, ResizeWindow);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Window::ResizeWindow(GLFWwindow* window, int width, int height)
{
	Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (myWindow)
	{
		myWindow->SetWidth(width);
		myWindow->SetHeight(height);
		glViewport(0, 0, width, height);
	}
}
