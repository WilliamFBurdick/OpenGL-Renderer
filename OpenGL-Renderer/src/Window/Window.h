#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Window
{
public:
	Window(int width = 1280, int height = 720, const char* title = "OpenGL Renderer");
	~Window();

	bool Init();

	inline void SetWidth(int width) { mWidth = width; }
	inline void SetHeight(int height) { mHeight = height; }

	void SetViewport(int x, int y, int w, int h);
	void GetViewport(int& x, int& y, int& w, int& h)
	{
		x = mViewportX; y = mViewportY; w = mViewportW; h = mViewportH;
	}

	inline int GetWidth() const { return mWidth; }
	inline int GetHeight() const { return mHeight; }
	inline GLFWwindow* GetWindow() const { return mWindow; }

	static void ResizeWindow(GLFWwindow* window, int width, int height);
private:
	int mWidth, mHeight;
	const char* mTitle;
	int mViewportX, mViewportY, mViewportW, mViewportH;

	GLFWwindow* mWindow;
};

