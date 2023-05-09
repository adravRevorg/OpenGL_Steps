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
void setup_objects();
void renderLoop();
void display();
void processInput(GLFWwindow* window);
void termination();
void setUniforms();
void refreshUniforms();



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
"uniform vec4 color;\n"
"void main() {\n"
"	FragColor = color;\n"
"}\n\0";


//VAO, VBO
unsigned int VAO, VBO, EBO;


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


void setup_objects() {
	
	//vertices and indices
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
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

	//share configuation
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	setUniforms();

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
	refreshUniforms();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

void setUniforms() {
	glUseProgram(shaderProgram);
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
}

void refreshUniforms() {
	float timeValue = glfwGetTime();
	float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
	glUniform4f(vertexColorLocation, colorValue, 0.0f, 0.0f, 1.0f);
}



/* Summary Notes

- Display a solid rectangle, with uniform color, changing as per time
- Use of uniforms
- Add uniform in fragment shader
- There's a limit to max number of uniforms that can be passed. Usually ?=16
- Set uniform by getting location, glUseProgram(), and using that to set value (in setUniforms())
- Dont forget to call glUseProgram if you havent already called
*/
