#pragma once

#include <glfw/glfw3.h>

class MyGlWindow
{
	friend void windowsSizeChangeCallback(GLFWwindow* window, int width, int height);
public:
	MyGlWindow();
	~MyGlWindow();

	int show();

	void run();

	int width();
	int height();

protected:
	void drawGL();

	void initializeGL();
	void refreshWindowSize();
private:
	GLFWwindow* _window;

	int _width;
	int _height;
};

