#include "gl/glew.h"
#include "MyGlWindow.h"

MyGlWindow::MyGlWindow()
{
}


MyGlWindow::~MyGlWindow()
{
}

int MyGlWindow::show()
{
	/* Intialize the library*/
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	_window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(_window);

	initializeGL();

	return 0;
}

void MyGlWindow::run()
{
	while (!glfwWindowShouldClose(_window))
	{
		drawGL();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void MyGlWindow::drawGL()
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MyGlWindow::initializeGL()
{
	glewExperimental = true;
	glewInit();
}
