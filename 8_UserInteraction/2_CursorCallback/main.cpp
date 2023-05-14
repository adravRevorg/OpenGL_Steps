#include <khrplatform.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;


//settings
#define WIDTH 800
#define HEIGHT 800

GLFWwindow* window;

//Functions
bool init();
void renderLoop();
void display();
void processInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);

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

	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, cursorCallback);

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
	glClearColor(xpos/WIDTH, 0.0f, 0.0f, 1.0f);
}


/* Summary Notes:

- Cursor Interactivity
  Change color as cursor moves
  Steps:
	-  Set the callback: glfwSetCursorPosCallback(window, cursorCallback);  
	-  Callback function has the following inputs:  void cursorCallback(GLFWwindow* window, double xpos, double ypos);
	-  set color as per x pos of cursor (note: remember to keep xpos/WIDTH, because we want 0-1 values for color
 
*/