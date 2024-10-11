#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
	Window(const char* title, unsigned int width = 1280, unsigned int height = 720);
	~Window();
	void Run();

	static void resize_window_callback(GLFWwindow* window, int width, int height);
private:
	GLFWwindow* m_Window;
	unsigned int m_Width, m_Height;
public:
	void SetDimensions(unsigned int w, unsigned int h) { m_Width = w; m_Height = h; }
	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
	inline GLFWwindow* GetWindow() const { return m_Window; }
};

