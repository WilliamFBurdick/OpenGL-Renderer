#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Mouse
{
	float xPos, yPos;
	float xDelta, yDelta;
	bool firstMouse;
};

class Input
{
public:
	static void ProcessInput(GLFWwindow* window);

	static void SetCursorVisible(GLFWwindow* window, bool visible = true);

	inline static int GetKey(GLFWwindow* window, int key) { return glfwGetKey(window, key); }
	inline static int GetMouseButton(GLFWwindow* window, int key) { return glfwGetMouseButton(window, key); }
	inline static Mouse& GetMouse() { return m_Mouse; }
private:
	static Mouse m_Mouse;
};

