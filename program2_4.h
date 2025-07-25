#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

#define numVAOs 1

class Program2_4 {
private:
	GLuint renderingProgram;
	GLuint vao[numVAOs];

	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();
	GLuint createShaderProgram();
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
	std::string readShaderSource(const char* filePath);

public:
	Program2_4();
	~Program2_4();
	void execute();
};