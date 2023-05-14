#include <khrplatform.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;


//settings
#define WIDTH 800
#define HEIGHT 800

bool changeColor = true;

GLFWwindow* window;

//Functions
bool init();
void renderLoop();
void display();
void processInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {

	if (!init())	return -1;

	renderLoop();

	glfwTerminate();

	return 0;

}


bool init() {

	//initialization, config
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window creation
	window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Window", NULL, NULL);
	if (!window) {
		cout << "Window creation failed";
		glfwTerminate();
		return false;
	}

	//context set
	glfwMakeContextCurrent(window);

	//glad loader
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to load GLAD";
		return false;
	}

	//setup callbacks
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetKeyCallback(window, keyCallback);

	return true;
}

void renderLoop() {

	while (!glfwWindowShouldClose(window)) {

		processInput(window);
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!changeColor)
		return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glClearColor(x / WIDTH, 0.0f, 0.0f, 1.0f);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glClearColor(0.0f, 1.0f - y/HEIGHT, 0.0f, 1.0f);
	}

}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	if (changeColor)
		glClearColor(xpos/WIDTH, 0.0f, 0.0f, 1.0f);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
		changeColor = !changeColor;
}


/* Summary Notes:

- Cursor Interactivity
  Change color as cursor moves / mouse clicked. But, press C to toggle this behaviour on/off
  Steps for key callback:
	-  Set the callback: glfwSetKeyCallback(window, keyCallback);
	-  Callback function has the following inputs:  keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	-  toggle on/off using changeColor variable
*/