#include "program7_1.h"

using namespace std;

void Program7_1::init(GLFWwindow* window) {
	brickTexture = file_utils::loadTexture("brick.jpg");

	renderingProgram = shader_utils::createShaderProgram("vertShader_7_4.glsl", "fragShader_7_4.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 10.0f;
	modelLocX = 0.0f; modelLocY = 0.0f; modelLocZ = 1.0f;
	//setupImportVertices();
	setupTorusVertices();
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}
void Program7_1::display(GLFWwindow* window, double currentTime) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram);

	// get the uniform variables for the MV and projection matrices
	pLoc = glGetUniformLocation(renderingProgram, "p_matrix");
	mLoc = glGetUniformLocation(renderingProgram, "m_matrix");
	vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

	// build view matrix, model matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(modelLocX, modelLocY, modelLocZ));
	// rotate the torus
	mMat *= glm::rotate(mMat, torusModel.toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// set up lights based on the current light’s position
	currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
	installLights();
	// build the inverse-transpose of the M matrix, for transforming normal vectors
	invTrMat = glm::transpose(glm::inverse(mMat));
	// put the M, V, P, and Inverse-transpose(normal) matrices into the corresponding uniforms
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));


	//// bind torus vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	// bind torus normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// texture
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// setup textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);


	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// bind indices of torus
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

	// for imported model
	//glDrawArrays(GL_TRIANGLES, 0, objModel.getNumVertices());

	// for torus
	glDrawElements(GL_TRIANGLES, torusModel.getNumIndices(), GL_UNSIGNED_INT, 0);
}

void Program7_1::execute() {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 800, "Chapter7 - program1", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int newWidth, int newHeight) {
		auto* self = static_cast<Program7_1*>(glfwGetWindowUserPointer(w));
		self->window_reshape_callback(newWidth, newHeight);
		});

	init(window);
	lastFrameTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		double now = glfwGetTime();
		float dt = static_cast<float>(now - lastFrameTime);
		lastFrameTime = now;

		glfwPollEvents();
		handleInput(window, dt);
		display(window, now);
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Program7_1::setupImportVertices(void) {
	std::vector<glm::vec3> vert = objModel.getVertices();
	std::vector<glm::vec2> tex = objModel.getTextureCoords();
	std::vector<glm::vec3> norm = objModel.getNormals();
	int numObjVertices = objModel.getNumVertices();


	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	// vertex positions
	 // texture coordinates
	 // normal vectors
	for (int i = 0; i < numObjVertices; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	// put the vertices into buffer #0
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	// put the texture coordinates into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	// put the normals into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
}

void Program7_1::setupTorusVertices(void){
	std::vector<int> ind = torusModel.getIndices();
	std::vector<glm::vec3> vert = torusModel.getVertices();
	std::vector<glm::vec2> tex = torusModel.getTexCoords();
	std::vector<glm::vec3> norm = torusModel.getNormals();
	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	// vertex positions
	 // texture coordinates
	 // normal vectors
	int numVertices = torusModel.getNumVertices();
	for (int i = 0; i < numVertices; i++) {
		pvalues.push_back(vert[i].x);
		pvalues.push_back(vert[i].y);
		pvalues.push_back(vert[i].z);
		tvalues.push_back(tex[i].s);
		tvalues.push_back(tex[i].t);
		nvalues.push_back(norm[i].x);
		nvalues.push_back(norm[i].y);
		nvalues.push_back(norm[i].z);
	}
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(4, vbo);
	// put the vertices into buffer #0
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	// put the texture coordinates into buffer #1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	// put the normals into buffer #2
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]); // indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);
}

void Program7_1::window_reshape_callback(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
}

void Program7_1::handleInput(GLFWwindow* window, float dt) {
	float v = moveSpeed * dt;

	// WASD: left/right, forward/back (world axes)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraZ -= v; // forward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraZ += v; // back
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraX -= v; // left
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraX += v; // right

	// Q/E: down/up
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) cameraY -= v; // down
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) cameraY += v; // up

	// Optional: ESC to quit
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Program7_1::installLights() {
	// save the light position in a float array
	lightPos[0] = currentLightPos.x;
	lightPos[1] = currentLightPos.y;
	lightPos[2] = currentLightPos.z;
	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");
	// set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mShiLoc, matShi);
}