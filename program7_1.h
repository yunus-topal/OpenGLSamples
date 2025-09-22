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
#include "Torus.h"

#define numVAOs 1  
#define numVBOs 4

class Program7_1 {
private:
	//----------------- Light properties -----------------//
	// locations for shader uniform variables
	GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
	glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
	float lightPos[3]; // light position as float array

	// initial light location
	glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);
	// white light properties
	float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// gold material properties
	float* matAmb = goldAmbient();
	float* matDif = goldDiffuse();
	float* matSpe = goldSpecular();
	float matShi = goldShininess();

	//----------------- Camera properties ----------------//
	float cameraX, cameraY, cameraZ;
	// for camera movement
	float moveSpeed = 6.0f;     // units per second
	double lastFrameTime = 0.0; // for delta time

	float modelLocX, modelLocY, modelLocZ;
	GLuint renderingProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];

	ImportedModel objModel = ImportedModel("teapot.obj");
	Torus torusModel = Torus(0.7f, 0.3f, 48);

	GLuint brickTexture;

	GLuint pLoc, mLoc, vLoc, nLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mMat, invTrMat;

	void setupImportVertices(void);
	void setupTorusVertices(void);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
	void window_reshape_callback(int newWidth, int newHeight);
	void handleInput(GLFWwindow* window, float dt);

	void installLights(void);

public:
	void execute();
};