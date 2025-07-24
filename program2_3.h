#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

class Program2_3 {
private:
	GLuint renderingProgram;
	GLuint vao[numVAOs];

	void printShaderLog(GLuint shader);
	void printProgramLog(int prog);
	bool checkOpenGLError();
	GLuint createShaderProgram();
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);

public:
	Program2_3();
	~Program2_3();
	void execute();
};