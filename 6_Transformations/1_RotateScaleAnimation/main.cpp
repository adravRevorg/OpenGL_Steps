#include <glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
void setupTexture(unsigned int texture, const char* path);
void renderLoop();
void display();
void processInput(GLFWwindow* window);
void termination();
void setupTransformation();
void updateTransformation();




//SHADERS
Shader* shader;

//VAO, VBO
unsigned int VAO, VBO, EBO;

//Texture
unsigned int texture[2];


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
}


void setup_objects() {

	//vertices(positions and colors) and indices
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind for VAO, VBO, EBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//share configuation (position and color)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, texture);
	
	setupTexture(texture[0], "texture1.png");

	shader->use();
	//setup texture units for the above two
	shader->setInt("texture1", 0);

	
	//setup Transformations
	setupTransformation();
	

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

	glClearColor(0.3f, 0.6f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	shader->use();
	//update transformation
	updateTransformation();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void termination() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader->ID);
	glfwTerminate();
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void setupTexture(unsigned int texture, const char* path) {


	//setp for 1st texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void setupTransformation() {

	//(Scale, and then rotate) => IRS    
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	//This: glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
	//or:
	shader->setMat4("transform", trans);
	
}

void updateTransformation() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians((float)sin(glfwGetTime()) * 90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(sin(glfwGetTime()), 0.5, 0.5));

	shader->setMat4("transform", trans);
}


/* Summary Notes
- Intro to transformations
- Use of GLM (OpenGL Mathematics)
- Changes required:
  Vertex shader: You need to receive a transformation matrix (uninform mat4), and multiply aPos with it
  main.cpp : You need to create  the transformation matrix and pass it as the uniform 
  Also, for animating the transformations, add updateTransformation() in display loop. We are, here, changing both the rotation and scaling as per time by passing a uniform

- Added new function setMat4 to shader class. Dont forget to include glm headers in shader.h as well

  To try? 
  What if we applied transformation to only a part of it?

  Also, there is color and some stuff in here that can be removed, but we might need it later, so, I am still keeping it here.
  Also, get stb_image.h from here: https://github.com/nothings/stb/blob/master/stb_image.h
*/