#include "Input.h"

Mouse Input::Cursor = Mouse();

void Input::Update(GLFWwindow* window)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (Cursor.bFirstMouse)
	{
		Cursor.xPos = xpos;
		Cursor.yPos = ypos;
		Cursor.bFirstMouse = false;
	}
	Cursor.xDelta = xpos - Cursor.xPos;
	Cursor.yDelta = ypos - Cursor.yPos;
	Cursor.xPos = xpos;
	Cursor.yPos = ypos;
}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{

}

void Input::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}

void Input::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
