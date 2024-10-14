#include "Input.h"

Mouse Input::m_Mouse = {0.0f, 0.0f, 0.0f, 0.0f, true};

void Input::ProcessInput(GLFWwindow* window)
{
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	if (m_Mouse.firstMouse)
	{
		m_Mouse.xPos = xPos;
		m_Mouse.yPos = yPos;
		m_Mouse.firstMouse = false;
	}

	m_Mouse.xDelta = (float)xPos - m_Mouse.xPos;
	m_Mouse.yDelta = (float)yPos - m_Mouse.yPos;

	m_Mouse.xPos = xPos;
	m_Mouse.yPos = yPos;
}

void Input::SetCursorVisible(GLFWwindow* window, bool visible)
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
