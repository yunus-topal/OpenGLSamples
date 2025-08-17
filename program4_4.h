#pragma once
#include <GL/glew.h>  
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>  
#include <iostream>  
#include <fstream>  
#include <string>  
#include <cmath>
#include <stack>
#include "utils.h"

#define numVAOs 1  
#define numVBOs 2  

class Program4_4 {
private:
	float cameraX, cameraY, cameraZ;
	float cubeLocX, cubeLocY, cubeLocZ;
	float pyrLocX, pyrLocY, pyrLocZ;
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];

	// allocate variables used in display() function, so that they won’t need to be allocated during rendering
	GLuint pLoc, mvLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mvMat;

	std::stack<glm::mat4> mvStack;

	void setupVertices(void);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
	void window_reshape_callback(int newWidth, int newHeight);

public:
	void execute();
};