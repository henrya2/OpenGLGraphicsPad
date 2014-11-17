#include "gl/glew.h"
#include "MyGlWindow.h"
#include <map>
#include <stdio.h>
#include <iostream>
#include <fstream>

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

bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		std::string buffer;
		buffer.resize(infoLogLength);
		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, &buffer[0]);

		std::cout << buffer << std::endl;

		return false;
	}

	return true;
}

bool checkShaderStatus(GLuint shaderId)
{

	return checkStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programId)
{
	return checkStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const std::string& fileName)
{
	std::ifstream inputStream(fileName.c_str());
	if (!inputStream)
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(inputStream),
		std::istreambuf_iterator<char>());
}

void initialShaders()
{
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	std::string shaderSourceCode;

	const char* adapter[1];
	shaderSourceCode = readShaderCode("../Data/Shaders/VertexShader.vert");
	adapter[0] = shaderSourceCode.c_str();
	glShaderSource(vertexShaderId, 1, adapter, 0);
	shaderSourceCode = readShaderCode("../Data/Shaders/FragmentShader.frag");
	adapter[0] = shaderSourceCode.c_str();
	glShaderSource(fragmentShaderId, 1, adapter, 0);

	glCompileShader(vertexShaderId);
	glCompileShader(fragmentShaderId);

	if (!checkShaderStatus(vertexShaderId) || !checkShaderStatus(fragmentShaderId))
		return;

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	if (!checkProgramStatus(programId))
		return;

	glUseProgram(programId);
}

const float X_DELTA = 0.1f;
const unsigned int NUM_VERTICES_PER_TRI = 3;
const unsigned int NUM_FLOATS_PER_VERTICE = 6;
const unsigned int TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
const unsigned int MAX_TRIS = 20;

unsigned int numTris = 0;

void sendDataToOpenGL()
{
	GLuint vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, MAX_TRIS * TRIANGLE_BYTE_SIZE, nullptr, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
}

void sendAnotherTriToOpenGL()
{
	if (numTris >= MAX_TRIS)
		return;

	const GLfloat THIS_TRI_X = -1 + numTris * X_DELTA;
	GLfloat thisTri[] =
	{
		THIS_TRI_X, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X + X_DELTA, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};
	glBufferSubData(GL_ARRAY_BUFFER, numTris * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, thisTri);

	numTris++;
}

void MyGlWindow::drawGL()
{
	glViewport(0, 0, width(), height());

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sendAnotherTriToOpenGL();
	glDrawArrays(GL_TRIANGLES, 0, numTris * NUM_VERTICES_PER_TRI);
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

void MyGlWindow::initializeGL()
{
	glewExperimental = true;
	GLenum result = glewInit();

	printf("glewInit return %d\n", result);

	glEnable(GL_DEPTH_TEST);

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
