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
#include "ImportedModel.h"

#define numVAOs 1  
#define numVBOs 4

class Program6_3 {
private:
	float cameraX, cameraY, cameraZ;
	// for camera movement
	float moveSpeed = 6.0f;     // units per second
	double lastFrameTime = 0.0; // for delta time

	float modelLocX, modelLocY, modelLocZ;
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];


	glm::vec3 lightPosWorld;
	glm::vec3 lightPosVS;
	ImportedModel objModel = ImportedModel("teapot.obj");

	GLuint pLoc, mvLoc, nLoc, lLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mvMat, nMat;
	std::stack<glm::mat4> mvStack;

	void setupVertices(void);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
	void window_reshape_callback(int newWidth, int newHeight);
	void handleInput(GLFWwindow* window, float dt);

public:
	void execute();
};