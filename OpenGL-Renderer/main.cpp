#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Globals.h"

const int DEFAULT_WIDTH = 800, DEFAULT_HEIGHT = 800;
void window_resize_callback(GLFWwindow* window, int newWidth, int newHeight);

void display(GLFWwindow* window)
{

}

void init(GLFWwindow* window)
{
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
}

int main()
{
	// initialize GLFW
	glfwInit();

	// set GLFW window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create GLFW window
	GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "OpenGL Renderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	glfwSetWindowSizeCallback(window, window_resize_callback);
	gladLoadGL();

	init(window);

	// set clear color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - totalTime;
		totalTime = currentTime;
		display(window);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void window_resize_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	screen_width = newWidth;
	screen_height = newHeight;
	glViewport(0, 0, newWidth, newHeight);
}