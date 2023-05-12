#ifndef SHADER_H
#define SHADER_H

#include <glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {

		string vertexShaderSrc_str, fragmentShaderSrc_str;
		
		ifstream vertexFile, fragmentFile;
		stringstream vertexStream, fragmentStream;

		try {
			vertexFile.open(vertexShaderPath);
			fragmentFile.open(fragmentShaderPath);

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			vertexFile.close();
			fragmentFile.close();

			vertexShaderSrc_str = vertexStream.str();
			fragmentShaderSrc_str = fragmentStream.str();

		}
		catch (...) {
			cout << "Read errors";
		}

		const char* vertexShaderSrc = vertexShaderSrc_str.c_str();
		const char* fragmentShaderSrc = fragmentShaderSrc_str.c_str();

		//Vertex Shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShader);
		//checkCompileErrors(vertextShader, 0);

		//Fragment Shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader,0);

		//Shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		//checkCompileErrors(ID, 1);

		//delete shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use() {
		glUseProgram(ID);
	}

private:

	void checkCompileErrors(unsigned int shader, int type) {
		int success;
		char infoLog[512];
		switch (type) {
		case 0:	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				cout << "Error" << infoLog;
			}
			break;

		case 1: glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 512, NULL, infoLog);
				cout << "Error" << infoLog;
			}
			break;
		}
	}


};
#endif