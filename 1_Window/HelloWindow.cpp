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
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}



/* Summary Notes:

- Include glad before glfw because glad includes required openGl headers

- Initialization
	- initialize the glfw library
	- create window hints - major, minor, core profile
	- create the window. check for successful creation, else glfwTerminate to release allocated resources
	- make the context as currrentr opengl context
	- load glad (glad manages all opengl function ptrs. so needed to load glad first)

- Render loop
	- while window should close (flag is glfwWindowSHouldClose)
	- process inpout
	- display stuff
	- swap buffers, poll events

- glfwTerminate to release resources

- so main function is: init, render, terminate
*/
