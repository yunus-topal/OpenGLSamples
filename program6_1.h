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
#include <vector>
#include "Sphere.h"

#define numVAOs 1  
#define numVBOs 3  

class Program6_1 {
private:
	float cameraX, cameraY, cameraZ;
	// for camera movement
	float moveSpeed = 6.0f;     // units per second
	double lastFrameTime = 0.0; // for delta time

	float cubeLocX, cubeLocY, cubeLocZ;
	float pyrLocX, pyrLocY, pyrLocZ;
	GLuint renderingProgram;
	GLuint brickTexture, useTexLoc;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];

	Sphere mySphere = Sphere(48);
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
	void handleInput(GLFWwindow* window, float dt);

public:
	void execute();
};