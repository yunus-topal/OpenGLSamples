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
#include "Sphere.h"

#define numVAOs 1  
#define numVBOs 4

class Program8_1 {
private:
	struct Material {
		float* ambient;
		float* diffuse;
		float* specular;
		float shininess;
	};
	//----------------- Light properties -----------------//
	// locations for shader uniform variables
	GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
	glm::vec3 currentLightPos; // light position as Vector3f, in both model and view space
	float lightPos[3]; // light position as float array

	// initial light location
	glm::vec3 initialLightPos = glm::vec3(0.0f, 4.0f, 1.0f);
	// white light properties
	float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// gold material properties
	Material goldMaterial = {
		goldAmbient(),
		goldDiffuse(),
		goldSpecular(),
		goldShininess()
	};
	Material bronzeMaterial = {
		bronzeAmbient(),
		bronzeDiffuse(),
		bronzeSpecular(),
		bronzeShininess()
	};

	//----------------- Camera properties ----------------//
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	// for camera movement
	float moveSpeed = 6.0f;     // units per second
	double lastFrameTime = 0.0; // for delta time

	glm::vec3 torusPos, spherePos;
	GLuint renderingProgram1;
	GLuint renderingProgram2;
	GLuint emissiveProgram;
	GLuint vao[numVAOs];
	GLuint vbo[numVBOs];

	GLuint vao2[numVAOs];
	GLuint vbo2[numVBOs];

	Torus torusModel = Torus(0.7f, 0.3f, 48);
	Sphere sphereModel = Sphere();

	GLuint pLoc, mLoc, vLoc, nLoc, sLoc;
	int width, height;
	float aspect;
	glm::mat4 pMat, vMat, mMat, invTrMat;


	// variables used in display() for transfering light to shaders
	float curAmb[4], curDif[4], curSpe[4];
	float curShi;

	// shadow-related variables 
	int screenSizeX, screenSizeY;
	GLuint shadowTex, shadowBuffer;
	glm::mat4 lightVmatrix;
	glm::mat4 lightPmatrix;
	glm::mat4 shadowMVP1;
	glm::mat4 shadowMVP2;
	glm::mat4 b;

	void setupTorusVertices(void);
	void setupSphereVertices(void);
	void init(GLFWwindow* window);
	void display(GLFWwindow* window, double currentTime);
	void window_reshape_callback(int newWidth, int newHeight);
	void handleInput(GLFWwindow* window, float dt);

	void installLights(GLuint renderingProgram, Material material);
	void setupShadowBuffers(GLFWwindow* window);
	void passOne(void);
	void passTwo(void);

public:
	void execute();
};