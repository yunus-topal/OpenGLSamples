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
#include "Torus.h"

#define numVAOs 1  
#define numVBOs 4

class Program6_2 {
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

	Torus myTorus = Torus(0.9, 0.5, 24, 10);
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