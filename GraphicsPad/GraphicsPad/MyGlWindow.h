#pragma once

#include <glfw/glfw3.h>

class MyGlWindow
{
public:
	MyGlWindow();
	~MyGlWindow();

	int show();

	void run();

protected:
	void drawGL();

	void initializeGL();

private:
	GLFWwindow* _window;
};

