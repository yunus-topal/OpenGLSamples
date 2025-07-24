#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

class Program2_2 {
protected:
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint createShaderProgram();
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);

public:
	Program2_2();
	virtual ~Program2_2();
	void execute();
};