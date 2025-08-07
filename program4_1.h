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
#include "utils.h"

#define numVAOs 1  
#define numVBOs 2  

class Program4_1 {  
private:  
	float cameraX, cameraY, cameraZ;
	float cubeLocX, cubeLocY, cubeLocZ;
	GLuint renderingProgram;  
	GLuint vao[numVAOs];  
	GLuint vbo[numVBOs];  

	// allocate variables used in display() function, so that they won’t need to be allocated during rendering
	GLuint mvLoc, pLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mMat, mvMat;

	float x = 0.0f;  
	float inc = 0.01f;  
	
	void setupVertices(void);
	std::string readShaderSource(const char* filePath);
	GLuint createShaderProgram();  
	void init(GLFWwindow* window);  
	void display(GLFWwindow* window, double currentTime);  

public:  
	Program4_1();  
	~Program4_1();  
	void execute();  
};