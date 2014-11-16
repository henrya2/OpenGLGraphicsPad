#include "gl/glew.h"
#include "MyGlWindow.h"
#include <map>
#include <stdio.h>

static std::map<void*, MyGlWindow*> g_mapedGlWindows;

static const int defaultWindowWidth = 640;
static const int defaultWindowHeight = 480;

static void windowsSizeChangeCallback(GLFWwindow* window, int width, int height)
{
	auto iter = g_mapedGlWindows.find(window);

	if (iter != g_mapedGlWindows.end())
	{
		iter->second->_width = width;
		iter->second->_height = height;
	}
}

static void registerGlWindow(GLFWwindow* window, MyGlWindow* glWindow)
{
	g_mapedGlWindows[window] = glWindow;
}

static void unRegisterGlWindow(GLFWwindow* window)
{
	g_mapedGlWindows.erase(window);
}

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

void sendDataToOpenGL()
{
	GLfloat verts[] =
	{
		0.0f, 0.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, +1.0f,
		+1.0f, +0.0f, +0.0f,
		-1.0f, +1.0f,
		+1.0f, +0.0f, +0.0f,
		-1.0f, -1.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, -1.0f,
		+1.0f, +0.0f, +0.0f
	};
	GLuint vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (char*)(2 * sizeof(float)));

	GLushort indices[] =
	{
		0, 1, 2,
		0, 3, 4
	};
	GLuint indexBufferId;
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void initialShaders()
{
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderId, 1, adapter, 0);
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderId, 1, adapter, 0);

	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderId);

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	glUseProgram(programId);
}

MyGlWindow::MyGlWindow()
{

}

MyGlWindow::~MyGlWindow()
{
	if (_window)
	{
		unRegisterGlWindow(_window);
	}
}

int MyGlWindow::show(const std::string& title, int width, int height)
{
	/* Intialize the library*/
	if (!glfwInit())
		return -1;

	int showWidth, showHeight;

	if (width < 0 || height < 0)
	{
		showWidth = defaultWindowWidth;
		showHeight = defaultWindowHeight;
	}
	else
	{
		showWidth = width;
		showHeight = height;
	}

	/* Create a windowed mode window and its OpenGL context */
	_window = glfwCreateWindow(showWidth, showHeight, title.c_str(), NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		return -1;
	}

	refreshWindowSize();

	glfwSetWindowSizeCallback(_window, &windowsSizeChangeCallback);

	registerGlWindow(_window, this);

	glfwMakeContextCurrent(_window);

	initializeGL();

	return 0;
}

void MyGlWindow::refreshWindowSize()
{
	glfwGetWindowSize(_window, &_width, &_height);
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
	glViewport(0, 0, width(), height());

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void MyGlWindow::initializeGL()
{
	glewExperimental = true;
	GLenum result = glewInit();

	printf("glewInit return %d\n", result);

	sendDataToOpenGL();
	initialShaders();
}

int MyGlWindow::width()
{
	return _width;
}

int MyGlWindow::height()
{
	return _height;
}
