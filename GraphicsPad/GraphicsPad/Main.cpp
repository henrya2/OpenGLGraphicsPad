#include "MyGlWindow.h"

int main()
{
	MyGlWindow myWindow;

	myWindow.show("GraphicsPad");
	myWindow.run();

	glfwTerminate();
	return 0;
}