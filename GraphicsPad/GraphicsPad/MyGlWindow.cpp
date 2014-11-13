#include "gl/glew.h"
#include "MyGlWindow.h"
#include <stdio.h>

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

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyGlWindow::initializeGL()
{
	glewExperimental = true;
	GLenum result = glewInit();

	printf("glewInit return %d\n", result);

	GLfloat verts[] =
	{
		+0.0f, +1.0f,
		-1.0f, -1.0f,
		+1.0f, -1.0f
	};
	GLuint myBufferId;
	glGenBuffers(1, &myBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}
