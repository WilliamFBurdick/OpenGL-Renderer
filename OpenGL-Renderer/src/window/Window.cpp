#include "Window.h"
#include <cstdio>
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>

Window::Window(const char* title, unsigned int width, unsigned int height):
	m_Width(width), m_Height(height), m_Window(nullptr)
{
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// create GLFW window
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window.");
	}
	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, Window::resize_window_callback);
	glfwSetWindowUserPointer(m_Window, this);

	// load all OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD.");
	}
}

Window::~Window()
{
}

void Window::Run()
{

}

void Window::resize_window_callback(GLFWwindow* window, int width, int height)
{
	Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (myWindow)
	{
		myWindow->SetDimensions(width, height);
	}
}