#pragma once

#include <GLFW/glfw3.h>

struct Mouse
{
	double xPos, yPos;
	double xDelta, yDelta;
	bool bFirstMouse;
};

class Input
{
public:
	static void Update(GLFWwindow* window);

	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static inline int GetKey(GLFWwindow* window, int key) { return glfwGetKey(window, key); }
	static inline int GetMouseButton(GLFWwindow* window, int button) { return glfwGetMouseButton(window, button); }

	static void SetMouseVisible(GLFWwindow* window, bool visible)
	{
		if (visible)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

	}

	static Mouse Cursor;
};

