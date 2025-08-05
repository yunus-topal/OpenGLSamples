#pragma once  
#include <GL/glew.h>  
#include <GLFW/glfw3.h>  
#include <iostream>  
#include <fstream>  
#include <string>  
#include "log_utils.h"  

#define numVAOs 1  
#define numVBOs 2  

class Program4_1 {  
private:  
	GLuint renderingProgram;  
	GLuint vao[numVAOs];  
	GLuint vbo[numVBOs];  

	float x = 0.0f;  
	float inc = 0.01f;  

	std::string readShaderSource(const char* filePath);
	GLuint createShaderProgram();  
	void init(GLFWwindow* window);  
	void display(GLFWwindow* window, double currentTime);  

public:  
	Program4_1();  
	~Program4_1();  
	void execute();  
};