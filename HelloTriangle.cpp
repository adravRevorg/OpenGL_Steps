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
void createShaders();
void setupVAO_VBO();
void renderLoop();
void display();
void processInput(GLFWwindow* window);
void termination();



//SHADERS
unsigned int shaderProgram;

//vertex shader
const char* vertexShaderSrc = "#version 330 core\n"

"layout(location = 0) in vec3 aPos;\n"

"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";


//fragment shader
const char* fragmentShaderSrc = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(0.5f);\n"
"}\n\0";


//VAO, VBO
unsigned int VAO, VBO;


int main() {

	if (!init()) return -1;

	createShaders();

	setupVAO_VBO();

	renderLoop();

	termination();
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

void createShaders() {

	//vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	int success; 
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		cout << "Couldnt compile vertex shader \n" << log << endl;
	}

	//fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		cout << "Couldnt compile vertex shader \n" << log << endl;
	}


	//Link Shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		cout << "Couldnt link shaders \n" << log << endl;
	}

	//Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void setupVAO_VBO() {
	
	//vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	//generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//share configuation
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void termination() {
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}



/* Summary Notes
- Displays a grey triangle
- Add shader source code

- init as usual in main

- creation of shaders
	- Vertex shader:
	  glCreateShader
	  glShaderSource
	  glCompileShader
	  glGetShaderiv, and glGetShaderInfoLog

	- Fragment shader
	  same as above

	- Link Shaders, so they receive i/p,o/p
	  glCreateProgram
	  glAttachShader (vert, frag)
	  glLinkProgram
	  glGetProgramiv, and glGetProgramInfoLog

	- delete vertex shader, fragment shader, no use

- VAO, VBO

	- define vertices array
	- generate vertex buffer, and vertex array object
	- bind vertex array, and bind array buffer to GL_ARRAY_BUFFER
	- share how to read
	- unbind buffer and array

- render loop
	- process i/p
	- display
		- bg color
		- glUseProgram to set shader in use
		- glBindVertexArray
		- glDrawArray
	- glfwSwapBuffer, glfwPollEvents
*/