#include "MyGlWindow.h"

int main()
{
	MyGlWindow myWindow;

	myWindow.show();
	myWindow.run();

	glfwTerminate();
	return 0;
}