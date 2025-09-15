#include "program6_3.h"
#include "Torus.h"

using namespace std;

void Program6_3::init(GLFWwindow* window) {
	/// TODO: fix lightning calculation in the shader
	//renderingProgram = shader_utils::createShaderProgram("vertShader_6_3.glsl", "fragShader_6_3.glsl");
	renderingProgram = shader_utils::createShaderProgram("vertShader_5_1.glsl", "fragShader_5_1.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 10.0f;
	modelLocX = 2.0f; modelLocY = 0.0f; modelLocZ = 1.0f;
	setupVertices();
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	lightPosWorld = glm::vec3(0.0f, 0.0f, 0.0f);
}
void Program6_3::display(GLFWwindow* window, double currentTime) {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(renderingProgram);
	glEnable(GL_CULL_FACE);

	// get the uniform variables for the MV and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	pLoc = glGetUniformLocation(renderingProgram, "p_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "n_matrix");
	lLoc = glGetUniformLocation(renderingProgram, "lightPosVS");

	// load perspective matrix
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// build view matrix, model matrix, and model-view matrix
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
	mvStack.push(vMat);


	// ----------------------  pyramid == sun  -------------------------------------------
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // sun position
	mvStack.push(mvStack.top());
	mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f)); // sun rotation

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	// pass normal matrix to the shader
	nMat = glm::transpose(glm::inverse(glm::mat3(mvMat)));
	glUniformMatrix3fv(nLoc, 1, GL_FALSE, glm::value_ptr(nMat));
	// pass light position in view space to the shader
	lightPosVS = glm::vec3(vMat * glm::vec4(lightPosWorld, 1.0f)); // vMat is your view
	glUniform3fv(lLoc, 1, glm::value_ptr(lightPosVS));

	//// bind torus vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//// bind torus texture coordinates buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// bind torus normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// bind indices of torus
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

	glDrawArrays(GL_TRIANGLES, 0, objModel.getNumVertices());
	mvStack.pop(); // pop the sun rotation matrix

	// sun position, and view matrices from stack
	mvStack.pop();  mvStack.pop();
}

void Program6_3::execute() {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 800, "Chapter6 - program3", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, [](GLFWwindow* w, int newWidth, int newHeight) {
		auto* self = static_cast<Program6_3*>(glfwGetWindowUserPointer(w));
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

void Program6_3::setupVertices(void) {
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

void Program6_3::window_reshape_callback(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees
}

void Program6_3::handleInput(GLFWwindow* window, float dt) {
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
