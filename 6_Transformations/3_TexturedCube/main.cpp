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
void updateTransformation();
void setUpCoordinateSystems();




//SHADERS
Shader* shader;

//VAO, VBO
unsigned int VAO, VBO;

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

	//vertices(positions and texture coords)
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	//generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind for VAO, VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//share configuation (position and texCoord)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, texture);
	
	setupTexture(texture[0], "texture1.png");

	shader->use();
	//setup texture units for the above two
	shader->setInt("texture1", 0);

	
	//setup Transformations
	setUpCoordinateSystems();
	

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
	//updateTransformation();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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


void updateTransformation() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians((float)sin(glfwGetTime()) * 90.0f), glm::vec3(0.0, 0.0, 1.0));

	shader->setMat4("transform", trans);
}

void setUpCoordinateSystems() {

	//Transformation matrix (simple identity matrix for now)
	glm::mat4 trans = glm::mat4(1.0f);
	shader->setMat4("transform", trans);

	//Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader->setMat4("model", model);

	//View matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	shader->setMat4("view", view);

	//Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	shader->setMat4("projection", projection);
}


/* Summary Notes
- Display a cube!
- Need to add vertices first
- Pass in vertex coords and texCoords to vertex shader
- We are not using element buffer yet, that is not using indices. So, be careful that you use glDrawArrays(), and not glDrawElements
- Also, updated the updateTransformation() to get a rotating cube. Uncomment that in display() to get animation

Okay, so we have a cube now
Next, 
- Use indices to write cleaner code
- Use z-buffer
- Try interpolated coloring
- Multiple cubes
*/