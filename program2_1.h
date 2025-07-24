#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Program2_1 {

private:
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
public:
	void execute();
};