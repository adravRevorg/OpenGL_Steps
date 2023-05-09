#include <khrplatform.h>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

#include <iostream>
using namespace std;


//settings
#define WIDTH 800
#define HEIGHT 800

GLFWwindow* window;

//Functions
bool init();
void createShaders();
void setup_objects();
void setup_VAO_VBO(unsigned int VAO, unsigned int VBO, unsigned int EBO, float vertices[], int vn, unsigned int indices[], int in);
void renderLoop();
void display();
void processInput(GLFWwindow* window);
void termination();




//SHADERS
unsigned int shaderProgram;
Shader* shader;

//VAO, VBO
unsigned int VAO[2], VBO[2], EBO;


int main() {

	if (!init()) return -1;

	createShaders();

	setup_objects();

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
	shader = new Shader("vertex.glsl", "fragment.glsl");
	shaderProgram = shader->ID;
}


void setup_objects() {
	
	//vertices(positions and colors) and indices
	float vertices[] = {
		// positions         // colors
		 -0.9f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		 -0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 -0.45f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	float vertices1[] = {
		//positions          //colors
		0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2
	};

	//generate
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	setup_VAO_VBO(VAO[0], VBO[0], EBO , vertices, sizeof(vertices), indices, sizeof(indices));
	setup_VAO_VBO(VAO[1], VBO[1], EBO , vertices1, sizeof(vertices1), indices, sizeof(indices));
	/*
	//bind for 1st VAO, VBO, EBO
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//share configuation (position and color)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//------------------------------------------

	//bind for 2nd VAO, VBO, EBO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//share configuation (position and color)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/


}

void setup_VAO_VBO(unsigned int VAO, unsigned int VBO, unsigned int EBO,  float vertices[], int vn, unsigned int indices[], int in) {

	cout << vn << " ";
	//bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vn, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in, indices, GL_STATIC_DRAW);

	//share configuation (position and color)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	shader->use();
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void termination() {
	glDeleteVertexArrays(2,VAO);
	glDeleteBuffers(2,VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


/* Summary Notes
- DRaw two triangles sing two VAO, and VBO
- VAO[2], VBO[2]
- glGenVertexArrays(2,VAO) -> generate two vao
- 
*/